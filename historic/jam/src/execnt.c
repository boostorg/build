/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

# include "jam.h"
# include "lists.h"
# include "execcmd.h"
# include "pathsys.h"
# include "debug.h"
# include <errno.h>
# include <assert.h>
# include <ctype.h>
# include <time.h>

# ifdef USE_EXECNT

# define WIN32_LEAN_AND_MEAN
# include <windows.h>		/* do the ugly deed */
# include <process.h>
# if !defined( __BORLANDC__ )
# include <tlhelp32.h>
# endif

# if !defined( __BORLANDC__ ) && !defined( OS_OS2 )
# define wait my_wait
static int my_wait( int *status );
# endif

/*
 * execnt.c - execute a shell command on Windows NT and Windows 95/98
 *
 * If $(JAMSHELL) is defined, uses that to formulate execvp()/spawnvp().
 * The default is:
 *
 *	/bin/sh -c %		[ on UNIX/AmigaOS ]
 *	cmd.exe /c %		[ on Windows NT ]
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

static int intr = 0;
static int cmdsrunning = 0;
static void (*istat)( int );

static int  is_nt_351        = 0;
static int  is_win95         = 1;
static int  is_win95_defined = 0;


static struct
{
	int	pid; /* on win32, a real process handle */
	void	(*func)( void *closure, int status, timing_info* );
	void 	*closure;
	char	*tempfile;

} cmdtab[ MAXJOBS ] = {{0}};


static void
set_is_win95( void )
{
  OSVERSIONINFO  os_info;

  os_info.dwOSVersionInfoSize = sizeof(os_info);
  os_info.dwPlatformId        = VER_PLATFORM_WIN32_WINDOWS;
  GetVersionEx( &os_info );
  
  is_win95         = (os_info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
  is_win95_defined = 1;
  
  /* now, test wether we're running Windows 3.51                */
  /* this is later used to limit the system call command length */
  if (os_info.dwPlatformId ==  VER_PLATFORM_WIN32_NT)
    is_nt_351 = os_info.dwMajorVersion == 3;
}

int maxline()
{
    if (!is_win95_defined)
        set_is_win95();
    
    /* Set the maximum command line length according to the OS */
    return is_nt_351 ? 996
        : is_win95 ? 1023
        : 2047;
}

static void
free_argv( char** args )
{
  free( args[0] );
  free( args );
}

/* Convert a command string into arguments for spawnvp.  The original
 * code, inherited from ftjam, tried to break up every argument on the
 * command-line, dealing with quotes, but that's really a waste of
 * time on Win32, at least.  It turns out that all you need to do is
 * get the raw path to the executable in the first argument to
 * spawnvp, and you can pass all the rest of the command-line
 * arguments to spawnvp in one, un-processed string.
 *
 * New strategy: break the string in at most one place.
 */
static char**
string_to_args( const char*  string )
{
    int src_len;
    int in_quote;
    char* line;
    char const* src;
    char* dst;
    char** argv;

    /* drop leading and trailing whitespace if any */
    while (isspace(*string))
        ++string;
  
    src_len = strlen( string );
    while ( src_len > 0 && isspace( string[src_len - 1] ) )
        --src_len;

    /* Copy the input string into a buffer we can modify
     */
    line = (char*)malloc( src_len+1 );
    if (!line)
        return 0;

    if ( DEBUG_PROFILE )
        profile_memory( src_len+1 );

    /* allocate the argv array.
     *   element 0: stores the path to the executable
     *   element 1: stores the command-line arguments to the executable
     *   element 2: NULL terminator
     */
    argv = (char**)malloc( 3 * sizeof(char*) );
    if (!argv)
    {
        free( line );
        return 0;
    }

    if ( DEBUG_PROFILE )
        profile_memory( 3 * sizeof(char*) );
    
    /* Strip quotes from the first command-line argument and find
     * where it ends.  Quotes are illegal in Win32 pathnames, so we
     * don't need to worry about preserving escaped quotes here.
     * Spaces can't be escaped in Win32, only enclosed in quotes, so
     * removing backslash escapes is also a non-issue.
     */
    in_quote = 0;
    for ( src = string, dst = line ; *src; src++ )
    {
        if (*src == '"')
            in_quote = !in_quote;
        else if (!in_quote && isspace(*src))
            break;
        else
            *dst++ = *src;
    }
    *dst++ = 0;
    argv[0] = line;

    /* skip whitespace in src */
    while (isspace(*src))
        ++src;

    argv[1] = dst;

	/* Copy the rest of the arguments verbatim */
    
    src_len -= src - string;

    /* Use strncat because it appends a trailing nul */
    *dst = 0;
    strncat(dst, src, src_len);

    argv[2] = 0;
    
    return argv;
}



/* process a "del" or "erase" command under Windows 95/98 */
static int
process_del( char*  command )
{
  char** arg;
  char*  p = command, *q;
  int    wildcard = 0, result = 0;

  /* first of all, skip the command itself */
  if ( p[0] == 'd' )
    p += 3; /* assumes "del..;" */
  else if ( p[0] == 'e' )
    p += 5; /* assumes "erase.." */
  else
    return 1; /* invalid command */

  /* process all targets independently */
  for (;;)
  {
    /* skip leading spaces */
    while ( *p && isspace(*p) )
      p++;
      
    /* exit if we encounter an end of string */
    if (!*p)
      return 0;
      
    /* ignore toggles/flags */
    while (*p == '/')
    {
      p++;
      while ( *p && isalnum(*p) )
          p++;
      while (*p && isspace(*p) )
          ++p;
    }

    
    {
      int  in_quote = 0;
      int  wildcard = 0;
      int  go_on    = 1;
      
      q = p;
      while (go_on)
      {
        switch (*p)
        {
          case '"':
            in_quote = !in_quote;
            break;
          
          case '?':
          case '*':
            if (!in_quote)
              wildcard = 1;
            break;
            
          case '\0':
            if (in_quote)
              return 1;
            /* fall-through */
              
          case ' ':
          case '\t':
            if (!in_quote)
            {
              int    len = p - q;
              int    result;
              char*  line;
              
              /* q..p-1 contains the delete argument */
              if ( len <= 0 )
                return 1;
  
              line = (char*)malloc( len+4+1 );
              if (!line)
                return 1;
              if ( DEBUG_PROFILE )
                  profile_memory( len+4+1 );
                
              strncpy( line, "del ", 4 );
              strncpy( line+4, q, len );
              line[len+4] = '\0';
              
              if ( wildcard )
                result = system( line );
              else
                result = !DeleteFile( line+4 );
  
              free( line );
              if (result)
                return 1;
                
              go_on = 0;
            }
            
          default:
            ;
        }
        p++;
      } /* while (go_on) */
    }
  }
}


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
 * can_spawn() - If the command is suitable for execution via spawnvp,
 * return a number >= the number of characters it would occupy on the
 * command-line.  Otherwise, return zero.
 */
long can_spawn(char* command)
{
    char *p;
    
    char inquote = 0;

    /* Move to the first non-whitespace */
    command += strspn( command, " \t" );

    p = command;
    
    /* Look for newlines and unquoted i/o redirection */
    do
    {
        p += strcspn( p, "'\n\"<>|" );

        switch (*p)
        {
        case '\n':
            /* skip over any following spaces */
            while( isspace( *p ) )
                ++p;
            /* Must use a .bat file if there is anything significant
             * following the newline
             */
            if (*p)
                return 0;
            break;
            
        case '"':
        case '\'':
            if (p > command && p[-1] != '\\')
            {
                if (inquote == *p)
                    inquote = 0;
                else if (inquote == 0)
                    inquote = *p;
            }
                
            ++p;
            break;
            
        case '<':
        case '>':
        case '|':
            if (!inquote)
                return 0;
            ++p;
            break;
        }
    }
    while (*p);

    /* Return the number of characters the command will occupy
     */
    return p - command;
}

void execnt_unit_test()
{
#if !defined(NDEBUG)        
    /* vc6 preprocessor is broken, so assert with these strings gets
     * confused. Use a table instead.
     */
    typedef struct test { char* command; int result; } test;
    test tests[] = {
        { "x", 0 },
        { "x\n ", 0 },
        { "x\ny", 1 },
        { "x\n\n y", 1 },
        { "echo x > foo.bar", 1 },
        { "echo x < foo.bar", 1 },
        { "echo x \">\" foo.bar", 0 },
        { "echo x \"<\" foo.bar", 0 },
        { "echo x \\\">\\\" foo.bar", 1 },
        { "echo x \\\"<\\\" foo.bar", 1 }
    };
    int i;
    for ( i = 0; i < sizeof(tests)/sizeof(*tests); ++i)
    {
        assert( !can_spawn( tests[i].command ) == tests[i].result );
    }

    {
        char* long_command = malloc(MAXLINE + 10);
        assert( long_command != 0 );
        memset( long_command, 'x', MAXLINE + 9 );
        long_command[MAXLINE + 9] = 0;
        assert( can_spawn( long_command ) == MAXLINE + 9);
        free( long_command );
    }

    {
        /* Work around vc6 bug; it doesn't like escaped string
         * literals inside assert
         */
        char** argv = string_to_args("\"g++\" -c -I\"Foobar\"");
        char const expected[] = "-c -I\"Foobar\""; 
        
        assert(!strcmp(argv[0], "g++"));
        assert(!strcmp(argv[1], expected));
        free_argv(argv);
    }
#endif 
}

/* 64-bit arithmetic helpers */

/* Compute the carry bit from the addition of two 32-bit unsigned numbers */
#define add_carry_bit(a, b) ( (((a) | (b)) >> 31) & (~((a) + (b)) >> 31) & 0x1 )

/* Compute the high 32 bits of the addition of two 64-bit unsigned numbers, h1l1 and h2l2 */
#define add_64_hi(h1, l1, h2, l2) ((h1) + (h2) + add_carry_bit(l1, l2))

/* Add two 64-bit unsigned numbers, h1l1 and h2l2 */
static FILETIME add_64(
    unsigned long h1, unsigned long l1,
    unsigned long h2, unsigned long l2)
{
    FILETIME result;
    result.dwLowDateTime = l1 + l2;
    result.dwHighDateTime = add_64_hi(h1, l1, h2, l2);

    return result;
}

static FILETIME add_FILETIME(FILETIME t1, FILETIME t2)
{
    return add_64(
        t1.dwHighDateTime, t1.dwLowDateTime
      , t2.dwHighDateTime, t2.dwLowDateTime);
}
static FILETIME negate_FILETIME(FILETIME t)
{
    /* 2s complement negation */
    return add_64(~t.dwHighDateTime, ~t.dwLowDateTime, 0, 1);
}

/* COnvert a FILETIME to a number of seconds */
static double filetime_seconds(FILETIME t)
{
    return t.dwHighDateTime * (double)(1UL << 31) * 2 + t.dwLowDateTime * 1.0e-7;
}

static void
record_times(int pid, timing_info* time)
{
    FILETIME creation, exit, kernel, user;
    if (GetProcessTimes((HANDLE)pid, &creation, &exit, &kernel, &user))
    {
        /* Compute the elapsed time */
#if 0 /* We don't know how to get this number this on Unix */
        time->elapsed = filetime_seconds(
            add_FILETIME( exit, negate_FILETIME(creation) )
        );
#endif 

        time->system = filetime_seconds(kernel);
        time->user = filetime_seconds(user);            
    }
        
    CloseHandle((HANDLE)pid);
}
    

/*
 * execcmd() - launch an async command execution
 */

void
execcmd( 
	char *string,
	void (*func)( void *closure, int status, timing_info* ),
	void *closure,
	LIST *shell )
{
    int pid;
    int slot;
    int raw_cmd = 0 ;
    char *argv_static[ MAXARGC + 1 ];	/* +1 for NULL */
    char **argv = argv_static;
    char *p;

    /* Check to see if we need to hack around the line-length limitation. */
    /* Look for a JAMSHELL setting of "%", indicating that the command
     * should be invoked directly */
    if ( shell && !strcmp(shell->string,"%") && !list_next(shell) )
    {
        raw_cmd = 1;
        shell = 0;
    }

    if ( !is_win95_defined )
        set_is_win95();
          
    /* Find a slot in the running commands table for this one. */
    if ( is_win95 )
    {
        /* only synchronous spans are supported on Windows 95/98 */
        slot = 0;
    }
    else
    {
        for( slot = 0; slot < MAXJOBS; slot++ )
            if( !cmdtab[ slot ].pid )
                break;
    }
    if( slot == MAXJOBS )
    {
        printf( "no slots for child!\n" );
        exit( EXITBAD );
    }
  
    if( !cmdtab[ slot ].tempfile )
    {
        const char *tempdir = path_tmpdir();
        DWORD procID = GetCurrentProcessId();
  
        /* SVA - allocate 64 other just to be safe */
        cmdtab[ slot ].tempfile = malloc( strlen( tempdir ) + 64 );
        if ( DEBUG_PROFILE )
            profile_memory( strlen( tempdir ) + 64 );
  
        sprintf( cmdtab[ slot ].tempfile, "%s\\jam%d-%02d.bat", 
                 tempdir, procID, slot );		
    }

    /* Trim leading, ending white space */

    while( isspace( *string ) )
        ++string;

    /* Write to .BAT file unless the line would be too long and it
     * meets the other spawnability criteria.
     */
    if( raw_cmd && can_spawn( string ) >= MAXLINE )
    {
        if( DEBUG_EXECCMD )
            printf("Executing raw command directly\n");        
    }
    else
    {
        FILE *f = 0;
        int tries = 0;
        raw_cmd = 0;
        
        /* Write command to bat file. For some reason this open can
           fails intermitently. But doing some retries works. Most likely
           this is due to a previously existing file of the same name that
           happens to be opened by an active virus scanner. Pointed out,
           and fix by Bronek Kozicki. */
        for (; !f && tries < 4; ++tries)
        {
            f = fopen( cmdtab[ slot ].tempfile, "w" );
            if ( !f && tries < 4 ) Sleep( 250 );
        }
        if (!f)
        {
            printf( "failed to write command file!\n" );
            exit( EXITBAD );
        }
        fputs( string, f );
        fclose( f );

        string = cmdtab[ slot ].tempfile;
        
        if( DEBUG_EXECCMD )
        {
            if (shell)
                printf("using user-specified shell: %s", shell->string);
            else
                printf("Executing through .bat file\n");
        }
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
    else if (raw_cmd)
    {
        argv = string_to_args(string);
    }
    else
    {
        /* don't worry, this is ignored on Win95/98, see later.. */
        argv[0] = "cmd.exe";
        argv[1] = "/Q/C";		/* anything more is non-portable */
        argv[2] = string;
        argv[3] = 0;
    }

    /* Catch interrupts whenever commands are running. */

    if( !cmdsrunning++ )
        istat = signal( SIGINT, onintr );

    /* Start the command */

    /* on Win95, we only do a synchronous call */
    if ( is_win95 )
    {
        static const char* hard_coded[] =
            {
                "del", "erase", "copy", "mkdir", "rmdir", "cls", "dir",
                "ren", "rename", "move", 0
            };
          
        const char**  keyword;
        int           len, spawn = 1;
        int           result;
        timing_info time = {0,0};
          
        for ( keyword = hard_coded; keyword[0]; keyword++ )
        {
            len = strlen( keyword[0] );
            if ( strnicmp( string, keyword[0], len ) == 0 &&
                 !isalnum(string[len]) )
            {
                /* this is one of the hard coded symbols, use 'system' to run */
                /* them.. except for "del"/"erase"                            */
                if ( keyword - hard_coded < 2 )
                    result = process_del( string );
                else
                    result = system( string );

                spawn  = 0;
                break;
            }
        }
          
        if (spawn)
        {
            char**  args;
            
            /* convert the string into an array of arguments */
            /* we need to take care of double quotes !!      */
            args = string_to_args( string );
            if ( args )
            {
#if 0
                char** arg;
                fprintf( stderr, "%s: ", args[0] );
                arg = args+1;
                while ( arg[0] )
                {
                    fprintf( stderr, " {%s}", arg[0] );
                    arg++;
                }
                fprintf( stderr, "\n" );
#endif              
                result = spawnvp( P_WAIT, args[0], args );
                record_times(result, &time);
                free_argv( args );
            }
            else
                result = 1;
        }
        func( closure, result ? EXEC_CMD_FAIL : EXEC_CMD_OK, &time );
        return;
    }

    if( DEBUG_EXECCMD )
    {
        char **argp = argv;

        printf("Executing command");
        while(*argp != 0)
        {
            printf(" [%s]", *argp);
            argp++;
        }
        printf("\n");
    }

    /* the rest is for Windows NT only */
    /* spawn doesn't like quotes around the command name */
    if ( argv[0][0] == '"')
    {
        int l = strlen(argv[0]);

        /* Clobber any closing quote, shortening the string by one
         * element */
        if (argv[0][l-1] == '"')
            argv[0][l-1] = '\0';
        
        /* Move everything *including* the original terminating zero
         * back one place in memory, covering up the opening quote */
        memmove(argv[0],argv[0]+1,l);
    }
    if( ( pid = spawnvp( P_NOWAIT, argv[0], argv ) ) == -1 )
    {
        perror( "spawn" );
        exit( EXITBAD );
    }
    /* Save the operation for execwait() to find. */

    cmdtab[ slot ].pid = pid;
    cmdtab[ slot ].func = func;
    cmdtab[ slot ].closure = closure;

    /* Wait until we're under the limit of concurrent commands. */
    /* Don't trust globs.jobs alone.                            */

    while( cmdsrunning >= MAXJOBS || cmdsrunning >= globs.jobs )
        if( !execwait() )
            break;
    
    if (argv != argv_static)
    {
        free_argv(argv);
    }
}

/*
 * execwait() - wait and drive at most one execution completion
 */

int
execwait()
{
	int i;
	int status, w;
	int rstat;
    timing_info time;

	/* Handle naive make1() which doesn't know if cmds are running. */

	if( !cmdsrunning )
	    return 0;

    if ( is_win95 )
        return 0;
          
	/* Pick up process pid and status */
    
    while( ( w = wait( &status ) ) == -1 && errno == EINTR )
        ;

	if( w == -1 )
	{
	    printf( "child process(es) lost!\n" );
	    perror("wait");
	    exit( EXITBAD );
	}

	/* Find the process in the cmdtab. */

	for( i = 0; i < MAXJOBS; i++ )
	    if( w == cmdtab[ i ].pid )
		break;

	if( i == MAXJOBS )
	{
	    printf( "waif child found!\n" );
	    exit( EXITBAD );
	}

    record_times(cmdtab[i].pid, &time);
    
	/* Clear the temp file */
    if ( cmdtab[i].tempfile )
        unlink( cmdtab[ i ].tempfile );

	/* Drive the completion */

	if( !--cmdsrunning )
	    signal( SIGINT, istat );

	if( intr )
	    rstat = EXEC_CMD_INTR;
	else if( w == -1 || status != 0 )
	    rstat = EXEC_CMD_FAIL;
	else
	    rstat = EXEC_CMD_OK;

	cmdtab[ i ].pid = 0;
	/* SVA don't leak temp files */
	if(cmdtab[i].tempfile != NULL)
	{
            free(cmdtab[i].tempfile);
            cmdtab[i].tempfile = NULL;
	}
	(*cmdtab[ i ].func)( cmdtab[ i ].closure, rstat, &time );

	return 1;
}

# if !defined( __BORLANDC__ )

/* The possible result codes from check_process_exit, below */
typedef enum { process_error, process_active, process_finished } process_state;

/* Helper for my_wait() below.  Checks to see whether the process has
 * exited and if so, records timing information.
 */
static process_state
check_process_exit(
    HANDLE process         /* The process we're looking at */
    
  , int* status            /* Storage for the finished process' exit
                            * code.  If the process is still active
                            * this location is left untouched. */
    
  , HANDLE* active_handles /* Storage for the process handle if it is
                            * found to be still active, or NULL.  The
                            * process is treated as though it is
                            * complete.  */
    
  , int* num_active        /* The current length of active_handles */
)
{
    DWORD exitcode;
    process_state result;

    /* Try to get the process exit code */
    if (!GetExitCodeProcess(process, &exitcode))
    {
        result = process_error; /* signal an error */
    }
    else if (
        exitcode == STILL_ACTIVE     /* If the process is still active */
        && active_handles != 0       /* and we've been passed a place to buffer it */
    )
    {
        active_handles[(*num_active)++] = process; /* push it onto the active stack */
        result = process_active;
    }
    else
    {
        *status = (int)((exitcode & 0xff) << 8);
        result = process_finished;
    }
    
    return result;
}

static double
running_time(HANDLE process)
{
    FILETIME creation, exit, kernel, user, current;
    if (GetProcessTimes(process, &creation, &exit, &kernel, &user))
    {
        /* Compute the elapsed time */
        GetSystemTimeAsFileTime(&current);
        {
            double delta = filetime_seconds(
                add_FILETIME( current, negate_FILETIME(creation) )
                );
            return delta;
        }
    }
    return 0.0;
}

/* it's just stupidly silly that one has to do this! */
typedef struct PROCESS_BASIC_INFORMATION__ {
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG AffinityMask;
    LONG BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
    } PROCESS_BASIC_INFORMATION_;
typedef LONG (__stdcall * NtQueryInformationProcess__)(
    HANDLE ProcessHandle,
    LONG ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength);
static NtQueryInformationProcess__ NtQueryInformationProcess_ = NULL;
static HMODULE NTDLL_ = NULL;
DWORD get_process_id(HANDLE process)
{
    PROCESS_BASIC_INFORMATION_ pinfo;
    if ( ! NtQueryInformationProcess_ )
    {
        if ( ! NTDLL_ )
        {
            NTDLL_ = GetModuleHandleA("ntdll");
        }
        if ( NTDLL_ )
        {
            NtQueryInformationProcess_
                = (NtQueryInformationProcess__)GetProcAddress( NTDLL_,"NtQueryInformationProcess" );
        }
    }
    if ( NtQueryInformationProcess_ )
    {
        LONG r = (*NtQueryInformationProcess_)(
            process,/* ProcessBasicInformation == */ 0,&pinfo,sizeof(PROCESS_BASIC_INFORMATION_),NULL);
        return pinfo.UniqueProcessId;
    }
    else
    {
        return 0;
    }
}

/* not really optimal, or efficient, but it's easier this way, and it's not
like we are going to be killing thousands, or even tens or processes. */
static void
kill_all(DWORD pid, HANDLE process)
{
    HANDLE process_snapshot_h = INVALID_HANDLE_VALUE;
    if ( !pid )
    {
        pid = get_process_id(process);
    }
    process_snapshot_h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    
    if (INVALID_HANDLE_VALUE != process_snapshot_h)
    {
        BOOL ok = TRUE;
        PROCESSENTRY32 pinfo;
        pinfo.dwSize = sizeof(PROCESSENTRY32);
        for (
            ok = Process32First(process_snapshot_h,&pinfo);
            TRUE == ok;
            ok = Process32Next(process_snapshot_h,&pinfo) )
        {
            if (pinfo.th32ParentProcessID == pid)
            {
                /* found a child, recurse to kill it and anything else below it */
                HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pinfo.th32ProcessID);
                if (NULL != ph)
                {
                    kill_all(pinfo.th32ProcessID,ph);
                    CloseHandle(ph);
                }
            }
        }
        CloseHandle(process_snapshot_h);
    }
    /* now that the children are all dead, kill the root */
    TerminateProcess(process,-2);
}

int is_parent_child(DWORD parent, DWORD child)
{
    HANDLE process_snapshot_h = INVALID_HANDLE_VALUE;

    if (parent == child)
        return 1;

    process_snapshot_h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if (INVALID_HANDLE_VALUE != process_snapshot_h)
    {
        BOOL ok = TRUE;
        PROCESSENTRY32 pinfo;
        pinfo.dwSize = sizeof(PROCESSENTRY32);
        for (
            ok = Process32First(process_snapshot_h, &pinfo); 
            ok == TRUE; 
            ok = Process32Next(process_snapshot_h, &pinfo) )
        {
            if (pinfo.th32ProcessID == child && pinfo.th32ParentProcessID)
                return is_parent_child(parent, pinfo.th32ParentProcessID);
        }

        CloseHandle(process_snapshot_h);
    }

    return 0;
}

int related(HANDLE h, DWORD p)
{
    return is_parent_child(get_process_id(h), p);
}

BOOL CALLBACK window_enum(HWND hwnd, LPARAM lParam)
{
    char buf[10] = {0};
    HANDLE h = *((HANDLE*) (lParam));
    DWORD pid = 0;

    if (!GetClassNameA(hwnd, buf, 10))
        return TRUE; // failed to read class name

    if (strcmp(buf, "#32770"))
        return TRUE; // not a dialog

    GetWindowThreadProcessId(hwnd, &pid);
    if (related(h, pid))
    {
        PostMessage(hwnd, WM_QUIT, 0, 0);
        // just one window at a time
        return FALSE;
    }

    return TRUE;
}

void close_alert(HANDLE process)
{
    EnumWindows(&window_enum, (LPARAM) &process);
}

static int
my_wait( int *status )
{
	int i, num_active = 0;
	DWORD exitcode, waitcode;
	HANDLE active_handles[MAXJOBS];

	/* first see if any non-waited-for processes are dead,
	 * and return if so.
	 */
	for ( i = 0; i < globs.jobs; i++ )
    {
        int pid = cmdtab[i].pid;
        
	    if ( pid )
        {
            process_state state
                = check_process_exit((HANDLE)pid, status, active_handles, &num_active);
            
            if ( state == process_error )
                goto FAILED;
            else if ( state == process_finished )
                return pid;
	    }
	}

	/* if a child exists, wait for it to die */
	if ( !num_active )
    {
	    errno = ECHILD;
	    return -1;
	}
    
    if ( globs.timeout > 0 )
    {
        /* with a timeout we wait for a finish or a timeout, we check every second
         to see if something timed out */
        for (waitcode = WAIT_TIMEOUT; waitcode == WAIT_TIMEOUT;)
        {
            waitcode = WaitForMultipleObjects( num_active, active_handles, FALSE, 1*1000 /* 1 second */ );
            if ( waitcode == WAIT_TIMEOUT )
            {
                /* check if any jobs have surpassed the maximum run time. */
                for ( i = 0; i < num_active; ++i )
                {
                    double t = running_time(active_handles[i]);
                    if ( t > (double)globs.timeout )
                    {
                        /* the job may have left an alert dialog around,
                         try and get rid of it before killing */
                        close_alert(active_handles[i]);
                        /* we have a "runaway" job, kill it */
                        kill_all(0,active_handles[i]);
                        /* indicate the job "finished" so we query its status below */
                        waitcode = WAIT_ABANDONED_0+i;
                    }
                }
            }
        }
    }
    else
    {
        /* no timeout, so just wait indefinately for something to finish */
        waitcode = WaitForMultipleObjects( num_active, active_handles, FALSE, INFINITE );
    }
	if ( waitcode != WAIT_FAILED )
    {
	    if ( waitcode >= WAIT_ABANDONED_0
             && waitcode < WAIT_ABANDONED_0 + num_active )
            i = waitcode - WAIT_ABANDONED_0;
	    else
            i = waitcode - WAIT_OBJECT_0;
        
        if ( check_process_exit(active_handles[i], status, 0, 0) == process_finished )
            return (int)active_handles[i];
	}

FAILED:
	errno = GetLastError();
	return -1;
    
}

# endif /* !__BORLANDC__ */

# endif /* USE_EXECNT */
