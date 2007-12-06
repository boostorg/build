/*
 * Copyright 1993, 1995 Christopher Seiwald.
 * Copyright 2007 Noel Belcourt.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "lists.h"
# include "execcmd.h"
# include "output.h"
# include <errno.h>
# include <signal.h>
# include <stdio.h>
# include <time.h>
# include <unistd.h> /* needed for vfork(), _exit() prototypes */
# include <sys/resource.h>
# include <sys/times.h>
# include <sys/wait.h>

#if defined(sun) || defined(__sun) || defined(linux)
#include <wait.h>
#endif

# ifdef USE_EXECUNIX
# include <sys/times.h>

# ifdef NO_VFORK
# define vfork() fork()
# endif

/*
 * execunix.c - execute a shell script on UNIX/WinNT/OS2/AmigaOS
 *
 * If $(JAMSHELL) is defined, uses that to formulate execvp()/spawnvp().
 * The default is:
 *
 *	/bin/sh -c %		[ on UNIX/AmigaOS ]
 *	cmd.exe /c %		[ on OS2/WinNT ]
 *
 * Each word must be an individual element in a jam variable value.
 *
 * In $(JAMSHELL), % expands to the command string and ! expands to 
 * the slot number (starting at 1) for multiprocess (-j) invocations.
 * If $(JAMSHELL) doesn't include a %, it is tacked on as the last
 * argument.
 *
 * Don't just set JAMSHELL to /bin/sh or cmd.exe - it won't work!
 *
 * External routines:
 *	execcmd() - launch an async command execution
 * 	execwait() - wait and drive at most one execution completion
 *
 * Internal routines:
 *	onintr() - bump intr to note command interruption
 *
 * 04/08/94 (seiwald) - Coherent/386 support added.
 * 05/04/94 (seiwald) - async multiprocess interface
 * 01/22/95 (seiwald) - $(JAMSHELL) support
 * 06/02/97 (gsar)    - full async multiprocess support for Win32
 */

static clock_t tps = 0;
static struct timeval tv;
static int select_timeout = 0;
static int intr = 0;
static int cmdsrunning = 0;
static struct tms old_time;

#define OUT 0
#define ERR 1

static struct
{
    int	    pid;              /* on win32, a real process handle */
    int     fd[2];            /* file descriptors for stdout and stderr */
    FILE   *stream[2];        /* child's stdout (0) and stderr (1) file stream */
    clock_t start_time;       /* start time of child process */
    int     exit_reason;      /* termination status */
    int     action_length;    /* length of action string */
    int     target_length;    /* length of target string */
    char   *action;           /* buffer to hold action and target invoked */
    char   *target;           /* buffer to hold action and target invoked */
    char   *command;          /* buffer to hold command being invoked */
    char   *buffer[2];        /* buffer to hold stdout and stderr, if any */
    void    (*func)( void *closure, int status, timing_info*, char *, char * );
    void   *closure;
    time_t  start_dt;         /* start of command timestamp */
} cmdtab[ MAXJOBS ] = {{0}};

/*
 * onintr() - bump intr to note command interruption
 */

void
onintr( int disp )
{
	intr++;
	printf( "...interrupted\n" );
}

/*
 * execcmd() - launch an async command execution
 */

void
execcmd( 
	char *string,
	void (*func)( void *closure, int status, timing_info*, char *, char * ),
	void *closure,
	LIST *shell,
        char *action,
        char *target )
{
        static int initialized = 0;
        int out[2], err[2];
	int slot, len;
	char *argv[ MAXARGC + 1 ];	/* +1 for NULL */

	/* Find a slot in the running commands table for this one. */

	for( slot = 0; slot < MAXJOBS; slot++ )
	    if( !cmdtab[ slot ].pid )
		break;

	if( slot == MAXJOBS )
	{
	    printf( "no slots for child!\n" );
	    exit( EXITBAD );
	}

	/* Forumulate argv */
	/* If shell was defined, be prepared for % and ! subs. */
	/* Otherwise, use stock /bin/sh (on unix) or cmd.exe (on NT). */

	if( shell )
	{
	    int i;
	    char jobno[4];
	    int gotpercent = 0;

	    sprintf( jobno, "%d", slot + 1 );

	    for( i = 0; shell && i < MAXARGC; i++, shell = list_next( shell ) )
	    {
		switch( shell->string[0] )
		{
		case '%':	argv[i] = string; gotpercent++; break;
		case '!':	argv[i] = jobno; break;
		default:	argv[i] = shell->string;
		}
		if( DEBUG_EXECCMD )
		    printf( "argv[%d] = '%s'\n", i, argv[i] );
	    }

	    if( !gotpercent )
		argv[i++] = string;

	    argv[i] = 0;
	}
	else
	{
	    argv[0] = "/bin/sh";
	    argv[1] = "-c";
	    argv[2] = string;
	    argv[3] = 0;
	}

	/* increment jobs running */
	++cmdsrunning;

        /* save off actual command string */
        cmdtab[ slot ].command = BJAM_MALLOC_ATOMIC(strlen(string)+1);
        strcpy(cmdtab[slot].command, string);

        /* initialize only once */
        if ( ! initialized ) {
          times(&old_time);
          initialized = 1;
        }

        /* create pipe from child to parent */

        if (pipe(out) < 0)
            exit(EXITBAD);

        fcntl(out[0], F_SETFL, O_NONBLOCK);
        fcntl(out[1], F_SETFL, O_NONBLOCK);

        if (pipe(err) < 0)
            exit(EXITBAD);

        fcntl(err[0], F_SETFL, O_NONBLOCK);
        fcntl(err[1], F_SETFL, O_NONBLOCK);

	/* Start the command */

        cmdtab[ slot ].start_dt = time(0);

        if (0 < globs.timeout) {
            /* 
             * handle hung processes by manually tracking elapsed 
             * time and signal process when time limit expires
             */
            struct tms buf;
            cmdtab[ slot ].start_time = times(&buf);

            /* make a global, only do this once */
            if (tps == 0) tps = sysconf(_SC_CLK_TCK);                
        }

	if ((cmdtab[slot].pid = vfork()) == 0) 
   	{
            close(out[0]);
            close(err[0]);

            dup2(out[1], STDOUT_FILENO);

            if (globs.pipe_action == 0)
            {
                dup2(out[1], STDERR_FILENO);
                close(err[1]);
            }
            else
                dup2(err[1], STDERR_FILENO);

            /* Make this process a process group leader
             * so that when we kill it, all child
             * processes of this process are terminated
             * as well.
             *
             * we use killpg(pid, SIGKILL) to kill the
             * process group leader and all its children.
             */
             if (0 < globs.timeout)
             {
                struct rlimit r_limit;
                r_limit.rlim_cur = globs.timeout;
                r_limit.rlim_max = globs.timeout;
                setrlimit(RLIMIT_CPU, &r_limit);
            }
            setpgid(cmdtab[slot].pid, cmdtab[slot].pid);

	    execvp( argv[0], argv );
	    _exit(127);
	}
        else if( cmdtab[slot].pid == -1 )
	{
	    perror( "vfork" );
	    exit( EXITBAD );
	}

        /* close write end of pipes */
        close(out[1]); 
        close(err[1]); 

        /* child writes stdout to out[1], parent reads from out[0] */
	cmdtab[slot].fd[OUT] = out[0];
	cmdtab[slot].stream[OUT] = fdopen(cmdtab[slot].fd[OUT], "rb");
        if (cmdtab[slot].stream[OUT] == NULL) {
            perror( "fdopen" );
            exit( EXITBAD );
        }

        /* child writes stderr to err[1], parent reads from err[0] */
        if (globs.pipe_action == 0)
        {
          close(err[0]);
        }
        else
        {
	    cmdtab[slot].fd[ERR] = err[0];
	    cmdtab[slot].stream[ERR] = fdopen(cmdtab[slot].fd[ERR], "rb");
            if (cmdtab[slot].stream[ERR] == NULL) {
                perror( "fdopen" );
                exit( EXITBAD );
            }
        }

        /* ensure enough room for rule and target name */

        if (action && target)
        {
            len = strlen(action) + 1;
            if (cmdtab[slot].action_length < len) 
            {
                BJAM_FREE(cmdtab[ slot ].action);
                cmdtab[ slot ].action = BJAM_MALLOC_ATOMIC(len);
                cmdtab[ slot ].action_length = len;
            }
            strcpy(cmdtab[ slot ].action, action);
            len = strlen(target) + 1;
            if (cmdtab[slot].target_length < len) 
            {
                BJAM_FREE(cmdtab[ slot ].target);
                cmdtab[ slot ].target = BJAM_MALLOC_ATOMIC(len);
                cmdtab[ slot ].target_length = len;
            }
            strcpy(cmdtab[ slot ].target, target);
        }
        else
        {
            BJAM_FREE(cmdtab[ slot ].action);
            BJAM_FREE(cmdtab[ slot ].target);
            cmdtab[ slot ].action = 0;
            cmdtab[ slot ].target = 0;
            cmdtab[ slot ].action_length = 0;
            cmdtab[ slot ].target_length = 0;
        }

	/* Save the operation for execwait() to find. */

	cmdtab[ slot ].func = func;
	cmdtab[ slot ].closure = closure;

	/* Wait until we're under the limit of concurrent commands. */
	/* Don't trust globs.jobs alone. */

        while( cmdsrunning >= MAXJOBS || cmdsrunning >= globs.jobs )
            if( !execwait() )
                break;
}

/* returns 1 if file is closed, 0 if descriptor is still live
 *
 * i is index into cmdtab
 *
 * s (stream) indexes 
 *
 * cmdtab[i].stream[s] 
 * cmdtab[i].buffer[s] and 
 * cmdtab[i].fd[s]
 */

int read_descriptor(int i, int s)
{
    int ret, len;
    char buffer[BUFSIZ];

    while (0 < (ret = fread(buffer, sizeof(char),  BUFSIZ-1, cmdtab[i].stream[s])))
    {
        buffer[ret] = 0;
        if (!cmdtab[i].buffer[s])
        {
            /* never been allocated */
            cmdtab[i].buffer[s] = (char*)BJAM_MALLOC_ATOMIC(ret+1);
            memcpy(cmdtab[i].buffer[s], buffer, ret+1);
        }
        else
        {
            /* previously allocated */
            char *tmp = cmdtab[i].buffer[s];
            len = strlen(tmp);
            cmdtab[i].buffer[s] = (char*)BJAM_MALLOC_ATOMIC(len+ret+1);
            memcpy(cmdtab[i].buffer[s], tmp, len);
            memcpy(cmdtab[i].buffer[s]+len, buffer, ret+1);
            BJAM_FREE(tmp);
        }
    }

    return feof(cmdtab[i].stream[s]);
}

void close_streams(int i, int s)
{
    /* close the stream and pipe descriptor */
    fclose(cmdtab[i].stream[s]);
    cmdtab[i].stream[s] = 0;
                                                                                  
    close(cmdtab[i].fd[s]);
    cmdtab[i].fd[s] = 0;
}

void populate_file_descriptors(int *fmax, fd_set *fds)
{
    int i, fd_max = 0;
    struct tms buf;
    clock_t current = times(&buf);
    select_timeout = globs.timeout;

    /* compute max read file descriptor for use in select */
    FD_ZERO(fds);
    for (i=0; i<globs.jobs; ++i)
    {
        if (0 < cmdtab[i].fd[OUT])
        {
            fd_max = fd_max < cmdtab[i].fd[OUT] ? cmdtab[i].fd[OUT] : fd_max;
            FD_SET(cmdtab[i].fd[OUT], fds);
        }
        if (globs.pipe_action != 0)
        {
            if (0 < cmdtab[i].fd[ERR])
            {
                fd_max = fd_max < cmdtab[i].fd[ERR] ? cmdtab[i].fd[ERR] : fd_max;
                FD_SET(cmdtab[i].fd[ERR], fds);
            }
        }

        if (globs.timeout && cmdtab[i].pid) {
            clock_t consumed = (current - cmdtab[i].start_time) / tps;
            clock_t process_timesout = globs.timeout - consumed;
            if (0 < process_timesout && process_timesout < select_timeout) {
                select_timeout = process_timesout;
            }
            if (globs.timeout <= consumed) {
                killpg(cmdtab[i].pid, SIGKILL);
                cmdtab[i].exit_reason = EXIT_TIMEOUT;
            }
        }
    }
    *fmax = fd_max;
}

/*
 * execwait() - wait and drive at most one execution completion
 */

int
execwait()
{
    int i, ret, fd_max;
    int pid, status, finished;
    int rstat;
    timing_info time_info;
    fd_set fds;
    struct tms new_time;

    /* Handle naive make1() which doesn't know if cmds are running. */

    if( !cmdsrunning )
        return 0;

    /* process children that signaled */
    finished = 0;
    while (!finished && cmdsrunning)
    {
        /* compute max read file descriptor for use in select */
        populate_file_descriptors(&fd_max, &fds);

        if (0 < globs.timeout) {
            /* force select to timeout so we can terminate expired processes */
            tv.tv_sec = select_timeout;
            tv.tv_usec = 0;

            /* select will wait until: io on a descriptor, a signal, or we time out */
            ret = select(fd_max+1, &fds, 0, 0, &tv);
        }
        else {
            /* select will wait until io on a descriptor or a signal */
            ret = select(fd_max+1, &fds, 0, 0, 0);
        }

        if (0 < ret)
        {
            for (i=0; i<globs.jobs; ++i)
            {
                int out = 0, err = 0;
                if (FD_ISSET(cmdtab[i].fd[OUT], &fds))
                    out = read_descriptor(i, OUT);

                if (FD_ISSET(cmdtab[i].fd[ERR], &fds))
                    err = read_descriptor(i, ERR);

                /* if feof on either descriptor, then we're done */
                if (out || err)
                {
                    /* close the stream and pipe descriptors */
                    close_streams(i, OUT);
                    if (globs.pipe_action != 0)
                        close_streams(i, ERR);

                    /* reap the child and release resources */
                    pid = waitpid(cmdtab[i].pid, &status, 0);

                    if (pid == cmdtab[i].pid)
                    {
                        finished = 1;
                        pid = 0;
                        cmdtab[i].pid = 0;

                        /* set reason for exit if not timed out */
                        if (WIFEXITED(status)) 
                        {
                            if (0 == WEXITSTATUS(status)) 
                                cmdtab[i].exit_reason = EXIT_OK;
                            else
                                cmdtab[i].exit_reason = EXIT_FAIL;
                        }

                        /* print out the rule and target name */
                        out_action(cmdtab[i].action, cmdtab[i].target,
                            cmdtab[i].command, cmdtab[i].buffer[OUT], cmdtab[i].buffer[ERR],
                            cmdtab[i].exit_reason);

                        times(&new_time);

                        time_info.system = (double)(new_time.tms_cstime - old_time.tms_cstime) / CLOCKS_PER_SEC;
                        time_info.user = (double)(new_time.tms_cutime - old_time.tms_cutime) / CLOCKS_PER_SEC;
                        time_info.start = cmdtab[i].start_dt;
                        time_info.end = time(0);
    
                        old_time = new_time;

                        /* Drive the completion */

                        --cmdsrunning;

                        if( intr )
                            rstat = EXEC_CMD_INTR;
                        else if( status != 0 )
                            rstat = EXEC_CMD_FAIL;
                        else
                            rstat = EXEC_CMD_OK;

                        /* assume -p0 in effect so only pass buffer[0] containing merged output */
                        (*cmdtab[ i ].func)( cmdtab[ i ].closure, rstat, &time_info, cmdtab[i].command, cmdtab[i].buffer[0] );

                        BJAM_FREE(cmdtab[i].buffer[OUT]);
                        cmdtab[i].buffer[OUT] = 0;

                        BJAM_FREE(cmdtab[i].buffer[ERR]);
                        cmdtab[i].buffer[ERR] = 0;

                        BJAM_FREE(cmdtab[i].command);
                        cmdtab[i].command = 0;

                        cmdtab[i].func = 0;
                        cmdtab[i].closure = 0;
                        cmdtab[i].start_time = 0;
                    }
                    else
                    {
                        printf("unknown pid %d with errno = %d\n", pid, errno);
                        exit(EXITBAD);
                    }
                }
            }
        }
    }

    return 1;
}

# endif /* USE_EXECUNIX */
