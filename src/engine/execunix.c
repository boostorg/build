/*
 * Copyright 1993, 1995 Christopher Seiwald.
 * Copyright 2007 Noel Belcourt.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

#include "jam.h"
#include "execcmd.h"

#include "lists.h"
#include "output.h"
#include "strings.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>  /* vfork(), _exit(), STDOUT_FILENO and such */
#include <sys/resource.h>
#include <sys/times.h>
#include <sys/wait.h>

#if defined(sun) || defined(__sun) || defined(linux)
    #include <wait.h>
#endif

#ifdef USE_EXECUNIX

#include <sys/times.h>

#if defined(__APPLE__)
    #define NO_VFORK
#endif

#ifdef NO_VFORK
    #define vfork() fork()
#endif


/*
 * execunix.c - execute a shell script on UNIX/OS2/AmigaOS
 *
 * If $(JAMSHELL) is defined, uses that to formulate execvp()/spawnvp(). The
 * default is: /bin/sh -c
 *
 * In $(JAMSHELL), % expands to the command string and ! expands to the slot
 * number (starting at 1) for multiprocess (-j) invocations. If $(JAMSHELL) does
 * not include a %, it is tacked on as the last argument.
 *
 * Each word must be an individual element in a jam variable value.
 *
 * Do not just set JAMSHELL to /bin/sh - it will not work!
 *
 * External routines:
 *  exec_cmd() - launch an async command execution.
 *  exec_wait() - wait for any of the async command processes to terminate.
 */

/* find a free slot in the running commands table */
static int get_free_cmdtab_slot();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static clock_t tps;
static int select_timeout;
static int cmdsrunning;
static int old_time_initialized;
static struct tms old_time;

/* We hold stdout & stderr child process information in two element arrays
 * indexed as follows.
 */
#define OUT 0
#define ERR 1

static struct
{
    int      pid;            /* on win32, a real process handle */
    int      fd[ 2 ];        /* file descriptors for stdout and stderr */
    FILE   * stream[ 2 ];    /* child's stdout (0) and stderr (1) file stream */
    clock_t  start_time;     /* start time of child process */
    int      exit_reason;    /* termination status */
    int      action_length;  /* length of action string */
    int      target_length;  /* length of target string */
    char   * action;         /* buffer to hold action and target invoked */
    char   * target;         /* buffer to hold action and target invoked */
    char   * command;        /* buffer to hold command being invoked */
    char   * buffer[ 2 ];    /* buffer to hold stdout and stderr, if any */
    int      buf_size[ 2 ];  /* size of buffer (bytes) */
    time_t   start_dt;       /* start of command timestamp */

    /* Function called when the command completes. */
    ExecCmdCallback func;

    /* Opaque data passed back to the 'func' callback. */
    void * closure;
} cmdtab[ MAXJOBS ] = { { 0 } };


/*
 * exec_cmd() - launch an async command execution.
 */

/* We hold file descriptors for pipes used to communicate with child processes
 * in a two element arrays indexed as follows.
 */
#define EXECCMD_PIPE_READ 0
#define EXECCMD_PIPE_WRITE 1

void exec_cmd
(
    string const * command,
    ExecCmdCallback func,
    void * closure,
    LIST * shell,
    char const * action,
    char const * target
)
{
    int const slot = get_free_cmdtab_slot();
    int out[ 2 ];
    int err[ 2 ];
    int len;
    char const * argv[ MAXARGC + 1 ];  /* +1 for NULL */

    /* Forumulate argv. If shell was defined, be prepared for % and ! subs.
     * Otherwise, use stock /bin/sh.
     */
    if ( !list_empty( shell ) )
    {
        int i;
        char jobno[ 4 ];
        int gotpercent = 0;
        LISTITER iter = list_begin( shell );
        LISTITER end = list_end( shell );

        sprintf( jobno, "%d", slot + 1 );

        for ( i = 0; iter != end && i < MAXARGC; ++i, iter = list_next( iter ) )
        {
            switch ( object_str( list_item( iter ) )[ 0 ] )
            {
                case '%': argv[ i ] = command->value; ++gotpercent; break;
                case '!': argv[ i ] = jobno; break;
                default : argv[ i ] = object_str( list_item( iter ) );
            }
            if ( DEBUG_EXECCMD )
                printf( "argv[%d] = '%s'\n", i, argv[ i ] );
        }

        if ( !gotpercent )
        argv[ i++ ] = command->value;

        argv[ i ] = 0;
    }
    else
    {
        argv[ 0 ] = "/bin/sh";
        argv[ 1 ] = "-c";
        argv[ 2 ] = command->value;
        argv[ 3 ] = 0;
    }

    /* Increment jobs running. */
    ++cmdsrunning;

    /* Save off actual command string. */
    cmdtab[ slot ].command = BJAM_MALLOC_ATOMIC( command->size + 1 );
    strcpy( cmdtab[ slot ].command, command->value );

    /* Create pipes from child to parent. */
    if ( pipe( out ) < 0 || pipe( err ) < 0 )
    {
        perror( "pipe" );
        exit( EXITBAD );
    }

    /* Initialize old_time only once. */
    if ( !old_time_initialized )
    {
        times( &old_time );
        old_time_initialized = 1;
    }

    /* Start the command */

    cmdtab[ slot ].start_dt = time( 0 );

    if ( 0 < globs.timeout )
    {
        /* Handle hung processes by manually tracking elapsed time and signal
         * process when time limit expires.
         */
        struct tms buf;
        cmdtab[ slot ].start_time = times( &buf );

        /* Make a global, only do this once. */
        if ( tps == 0 ) tps = sysconf( _SC_CLK_TCK );
    }

    /* Child does not need the read pipe ends used by the parent. */
    fcntl( out[ EXECCMD_PIPE_READ ], F_SETFD, FD_CLOEXEC );
    fcntl( err[ EXECCMD_PIPE_READ ], F_SETFD, FD_CLOEXEC );

    if ( ( cmdtab[ slot ].pid = vfork() ) == -1 )
    {
        perror( "vfork" );
        exit( EXITBAD );
    }

    if ( cmdtab[ slot ].pid == 0 )
    {
        /*****************/
        /* Child process */
        /*****************/
        int const pid = getpid();

        /* Redirect stdout and stderr to pipes inherited from the parent. */
        dup2( out[ EXECCMD_PIPE_WRITE ], STDOUT_FILENO );
        dup2( globs.pipe_action ? err[ EXECCMD_PIPE_WRITE ] :
            out[ EXECCMD_PIPE_WRITE ], STDERR_FILENO );
        close( out[ EXECCMD_PIPE_WRITE ] );
        close( err[ EXECCMD_PIPE_WRITE ] );

        /* Make this process a process group leader so that when we kill it, all
         * child processes of this process are terminated as well. We use
         * killpg( pid, SIGKILL ) to kill the process group leader and all its
         * children.
         */
        if ( 0 < globs.timeout )
        {
            struct rlimit r_limit;
            r_limit.rlim_cur = globs.timeout;
            r_limit.rlim_max = globs.timeout;
            setrlimit( RLIMIT_CPU, &r_limit );
        }
        setpgid( pid, pid );
        execvp( argv[ 0 ], (char * *)argv );
        perror( "execvp" );
        _exit( 127 );
    }

    /******************/
    /* Parent process */
    /******************/
    setpgid( cmdtab[ slot ].pid, cmdtab[ slot ].pid );

    /* Close pipe write ends. */
    close( out[ EXECCMD_PIPE_WRITE ] );
    close( err[ EXECCMD_PIPE_WRITE ] );

    /* Set both file descriptors to non-blocking. */
    fcntl( out[ EXECCMD_PIPE_READ ], F_SETFL, O_NONBLOCK );
    fcntl( err[ EXECCMD_PIPE_READ ], F_SETFL, O_NONBLOCK );

    /* Child writes stdout to out[ EXECCMD_PIPE_WRITE ], parent reads from
     * out[ EXECCMD_PIPE_READ ].
     */
    cmdtab[ slot ].fd[ OUT ] = out[ EXECCMD_PIPE_READ ];
    cmdtab[ slot ].stream[ OUT ] = fdopen( cmdtab[ slot ].fd[ OUT ], "rb" );
    if ( !cmdtab[ slot ].stream[ OUT ] )
    {
        perror( "fdopen" );
        exit( EXITBAD );
    }

    /* Child writes stderr to err[ EXECCMD_PIPE_WRITE ], parent reads from
     * err[ EXECCMD_PIPE_READ ].
     */
    if ( globs.pipe_action )
    {
        cmdtab[ slot ].fd[ ERR ] = err[ EXECCMD_PIPE_READ ];
        cmdtab[ slot ].stream[ ERR ] = fdopen( cmdtab[ slot ].fd[ ERR ], "rb" );
        if ( !cmdtab[ slot ].stream[ ERR ] )
        {
            perror( "fdopen" );
            exit( EXITBAD );
        }
    }
    else
        close( err[ EXECCMD_PIPE_READ ] );

    /* Save input data into the selected running commands table slot. */
    cmdtab[ slot ].func = func;
    cmdtab[ slot ].closure = closure;

    /* Ensure enough room for rule and target name. */
    if ( action && target )
    {
        len = strlen( action ) + 1;
        if ( cmdtab[ slot ].action_length < len )
        {
            BJAM_FREE( cmdtab[ slot ].action );
            cmdtab[ slot ].action = BJAM_MALLOC_ATOMIC( len );
            cmdtab[ slot ].action_length = len;
        }
        strcpy( cmdtab[ slot ].action, action );
        len = strlen( target ) + 1;
        if ( cmdtab[ slot ].target_length < len )
        {
            BJAM_FREE( cmdtab[ slot ].target );
            cmdtab[ slot ].target = BJAM_MALLOC_ATOMIC( len );
            cmdtab[ slot ].target_length = len;
        }
        strcpy( cmdtab[ slot ].target, target );
    }
    else
    {
        BJAM_FREE( cmdtab[ slot ].action );
        BJAM_FREE( cmdtab[ slot ].target );
        cmdtab[ slot ].action = 0;
        cmdtab[ slot ].target = 0;
        cmdtab[ slot ].action_length = 0;
        cmdtab[ slot ].target_length = 0;
    }

    /* Wait until we are under the limit of concurrent commands. Do not trust
     * globs.jobs alone.
     */
    while ( ( cmdsrunning >= MAXJOBS ) || ( cmdsrunning >= globs.jobs ) )
        if ( !exec_wait() )
            break;
}

#undef EXECCMD_PIPE_READ
#undef EXECCMD_PIPE_WRITE


/* Returns 1 if file descriptor is closed, or 0 if it is still alive.
 *
 * i is index into cmdtab
 *
 * s (stream) indexes:
 *  - cmdtab[ i ].stream[ s ]
 *  - cmdtab[ i ].buffer[ s ]
 *  - cmdtab[ i ].fd    [ s ]
 */

int read_descriptor( int i, int s )
{
    int ret;
    char buffer[ BUFSIZ ];

    while ( 0 < ( ret = fread( buffer, sizeof( char ), BUFSIZ - 1,
        cmdtab[ i ].stream[ s ] ) ) )
    {
        buffer[ ret ] = 0;
        if ( !cmdtab[ i ].buffer[ s ] )
        {
            /* Never been allocated. */
            if ( globs.max_buf && ret > globs.max_buf )
            {
                ret = globs.max_buf;
                buffer[ ret ] = 0;
            }
            cmdtab[ i ].buf_size[ s ] = ret + 1;
            cmdtab[ i ].buffer[ s ] = (char*)BJAM_MALLOC_ATOMIC( ret + 1 );
            memcpy( cmdtab[ i ].buffer[ s ], buffer, ret + 1 );
        }
        else
        {
            /* Previously allocated. */
            if ( cmdtab[ i ].buf_size[ s ] < globs.max_buf || !globs.max_buf )
            {
                char * tmp = cmdtab[ i ].buffer[ s ];
                int const old_len = cmdtab[ i ].buf_size[ s ] - 1;
                int const new_len = old_len + ret + 1;
                cmdtab[ i ].buf_size[ s ] = new_len;
                cmdtab[ i ].buffer[ s ] = (char*)BJAM_MALLOC_ATOMIC( new_len );
                memcpy( cmdtab[ i ].buffer[ s ], tmp, old_len );
                memcpy( cmdtab[ i ].buffer[ s ] + old_len, buffer, ret + 1 );
                BJAM_FREE( tmp );
            }
        }
    }

    return feof( cmdtab[ i ].stream[ s ] );
}


/*
 * close_streams() - Close the stream and pipe descriptor.
 */

void close_streams( int const i, int const s )
{
    fclose( cmdtab[ i ].stream[ s ] );
    cmdtab[ i ].stream[ s ] = 0;

    close( cmdtab[ i ].fd[ s ] );
    cmdtab[ i ].fd[ s ] = 0;
}


void populate_file_descriptors( int * const fmax, fd_set * const fds )
{
    int i;
    int fd_max = 0;
    struct tms buf;
    clock_t current = times( &buf );
    select_timeout = globs.timeout;

    /* Compute max read file descriptor for use in select. */
    FD_ZERO( fds );
    for ( i = 0; i < globs.jobs; ++i )
    {
        if ( 0 < cmdtab[ i ].fd[ OUT ] )
        {
            if ( fd_max < cmdtab[ i ].fd[ OUT ] )
                fd_max = cmdtab[ i ].fd[ OUT ];
            FD_SET( cmdtab[ i ].fd[ OUT ], fds );
        }
        if ( globs.pipe_action )
        {
            if ( 0 < cmdtab[ i ].fd[ ERR ] )
            {
                if ( fd_max < cmdtab[ i ].fd[ ERR ] )
                    fd_max = cmdtab[ i ].fd[ ERR ];
                FD_SET( cmdtab[ i ].fd[ ERR ], fds );
            }
        }

        if ( globs.timeout && cmdtab[ i ].pid )
        {
            clock_t consumed = ( current - cmdtab[ i ].start_time ) / tps;
            clock_t process_timesout = globs.timeout - consumed;
            if ( 0 < process_timesout && process_timesout < select_timeout )
                select_timeout = process_timesout;
            if ( globs.timeout <= consumed )
            {
                killpg( cmdtab[ i ].pid, SIGKILL );
                cmdtab[ i ].exit_reason = EXIT_TIMEOUT;
            }
        }
    }
    *fmax = fd_max;
}


/*
 * exec_wait() - wait for any of the async command processes to terminate.
 *
 * May register more than one terminated child process but will exit as soon as
 * at least one has been registered.
 */

int exec_wait()
{
    int fd_max;
    int finished = 0;
    fd_set fds;

    /* Handle naive make1() which does not know if commands are running. */
    if ( !cmdsrunning )
        return 0;

    /* Process children that signaled. */
    while ( !finished && cmdsrunning )
    {
        int i;
        int ret;
        struct timeval tv;
        struct timeval * ptv = NULL;

        /* Compute max read file descriptor for use in select(). */
        populate_file_descriptors( &fd_max, &fds );

        /* Force select() to timeout so we can terminate expired processes. */
        if ( 0 < globs.timeout )
        {
            tv.tv_sec = select_timeout;
            tv.tv_usec = 0;
            ptv = &tv;
        }

        /* select() will wait for I/O on a descriptor, a signal, or timeout. */
        while ( ( ret = select( fd_max + 1, &fds, 0, 0, ptv ) ) == -1 )
            if ( errno != EINTR )
                break;
        if ( ret <= 0 )
            continue;

        for ( i = 0; i < globs.jobs; ++i )
        {
            int out = 0;
            int err = 0;
            if ( FD_ISSET( cmdtab[ i ].fd[ OUT ], &fds ) )
                out = read_descriptor( i, OUT );

            if ( globs.pipe_action && FD_ISSET( cmdtab[ i ].fd[ ERR ], &fds ) )
                err = read_descriptor( i, ERR );

            /* If feof on either descriptor, we are done. */
            if ( out || err )
            {
                int pid;
                int status;
                int rstat;
                timing_info time_info;
                struct tms new_time;

                /* Close the stream and pipe descriptors. */
                close_streams( i, OUT );
                if ( globs.pipe_action != 0 )
                    close_streams( i, ERR );

                /* Reap the child and release resources. */
                while ( ( pid = waitpid( cmdtab[ i ].pid, &status, 0 ) ) == -1 )
                    if ( errno != EINTR )
                        break;

                if ( pid != cmdtab[ i ].pid )
                {
                    printf( "unknown pid %d with errno = %d\n", pid, errno );
                    exit( EXITBAD );
                }

                finished = 1;
                cmdtab[ i ].pid = 0;

                /* Set reason for exit if not timed out. */
                if ( WIFEXITED( status ) )
                    cmdtab[ i ].exit_reason = WEXITSTATUS( status )
                        ? EXIT_FAIL
                        : EXIT_OK;

                /* Print out the rule and target name. */
                out_action( cmdtab[ i ].action, cmdtab[ i ].target,
                    cmdtab[ i ].command, cmdtab[ i ].buffer[ OUT ],
                    cmdtab[ i ].buffer[ ERR ], cmdtab[ i ].exit_reason );

                times( &new_time );
                time_info.system = (double)( new_time.tms_cstime - old_time.tms_cstime ) / CLOCKS_PER_SEC;
                time_info.user   = (double)( new_time.tms_cutime - old_time.tms_cutime ) / CLOCKS_PER_SEC;
                time_info.start  = cmdtab[ i ].start_dt;
                time_info.end    = time( 0 );
                old_time = new_time;

                /* Drive the completion. */
                --cmdsrunning;

                if ( interrupted() )
                    rstat = EXEC_CMD_INTR;
                else if ( status )
                    rstat = EXEC_CMD_FAIL;
                else
                    rstat = EXEC_CMD_OK;

                /* Call the callback, may call back to jam rule land. Assume -p0
                 * is in effect so only pass buffer[ 0 ] containing merged
                 * output.
                 */
                (*cmdtab[ i ].func)( cmdtab[ i ].closure, rstat, &time_info,
                    cmdtab[ i ].command, cmdtab[ i ].buffer[ 0 ] );

                BJAM_FREE( cmdtab[ i ].buffer[ OUT ] );
                cmdtab[ i ].buffer[ OUT ] = 0;
                cmdtab[ i ].buf_size[ OUT ] = 0;

                BJAM_FREE( cmdtab[ i ].buffer[ ERR ] );
                cmdtab[ i ].buffer[ ERR ] = 0;
                cmdtab[ i ].buf_size[ ERR ] = 0;

                BJAM_FREE( cmdtab[ i ].command );
                cmdtab[ i ].command = 0;

                cmdtab[ i ].func = 0;
                cmdtab[ i ].closure = 0;
                cmdtab[ i ].start_time = 0;
            }
        }
    }

    return 1;
}


/*
 * Find a free slot in the running commands table.
 */

static int get_free_cmdtab_slot()
{
    int slot;
    for ( slot = 0; slot < MAXJOBS; ++slot )
        if ( !cmdtab[ slot ].pid )
            return slot;
    printf( "no slots for child!\n" );
    exit( EXITBAD );
}

# endif /* USE_EXECUNIX */
