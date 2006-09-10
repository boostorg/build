/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "builtins.h"
# include "rules.h"
# include "filesys.h"
# include "newstr.h"
# include "regexp.h"
# include "frames.h"
# include "hash.h"
# include "strings.h"
# include "pwd.h"
# include "pathsys.h"
# include "make.h"
# include "hdrmacro.h"
# include "compile.h"
# include "native.h"
# include "variable.h"
# include <ctype.h>

/*
 * builtins.c - builtin jam rules
 *
 * External routines:
 *
 * 	load_builtin() - define builtin rules
 *
 * Internal routines:
 *
 *	builtin_depends() - DEPENDS/INCLUDES rule
 *	builtin_echo() - ECHO rule
 *	builtin_exit() - EXIT rule
 *	builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *	builtin_glob() - GLOB rule
 *	builtin_match() - MATCH rule
 *
 * 01/10/01 (seiwald) - split from compile.c
 */

/*
 * compile_builtin() - define builtin rules
 */

# define P0 (PARSE *)0
# define C0 (char *)0

# if defined( OS_NT ) || defined( OS_CYGWIN )
LIST* builtin_system_registry( PARSE *parse, FRAME *frame );
LIST* builtin_system_registry_names( PARSE *parse, FRAME *frame );
# endif

int glob( char *s, char *c );

void lol_build( LOL* lol, char** elements );
void backtrace( FRAME *frame );
void backtrace_line( FRAME *frame );
void print_source_line( PARSE* p );

RULE* bind_builtin( char* name, LIST*(*f)(PARSE*, FRAME*), int flags, char** args )
{
    argument_list* arg_list = 0;
    
    if ( args )
    {
        arg_list = args_new();
        lol_build( arg_list->data, args );
    }

    return new_rule_body( root_module(), name, arg_list,
                          parse_make( f, P0, P0, P0, C0, C0, flags ), 1 );
}

RULE* duplicate_rule( char* name, RULE* other )
{
    return import_rule( other, root_module(), name );
}

void
load_builtins()
{
    duplicate_rule( "Always" ,
      bind_builtin( "ALWAYS" ,
                    builtin_flags, T_FLAG_TOUCHED, 0 ) );

    duplicate_rule( "Depends" ,
      bind_builtin( "DEPENDS" ,
                    builtin_depends, 0, 0 ) );

    duplicate_rule( "echo" ,
    duplicate_rule( "Echo" ,
      bind_builtin( "ECHO" ,
                    builtin_echo, 0, 0 ) ) );


    {
        char * args[] = { "message", "*", ":", "result-value", "?", 0 };
        duplicate_rule( "exit" ,
        duplicate_rule( "Exit" ,
          bind_builtin( "EXIT" ,
                        builtin_exit, 0, args ) ) );
    }

    {
        char * args[] = { "directories", "*", ":", "patterns", "*", ":", "case-insensitive", "?", 0 };
        duplicate_rule(
            "Glob" ,
            bind_builtin( "GLOB" , builtin_glob, 0, args )
            );
    }

    {
        char * args[] = { "patterns", "*", 0 };
        bind_builtin( "GLOB-RECURSIVELY" , builtin_glob_recursive, 0, args );
    }


    duplicate_rule( "Includes" ,
      bind_builtin( "INCLUDES" ,
                    builtin_depends, 1, 0 ) );

    {
        char * args[] = { "targets", "*", ":", "targets-to-rebuild", "*", 0 };
        bind_builtin( "REBUILDS" ,
                      builtin_rebuilds, 0, args );
    }
    
    duplicate_rule( "Leaves" ,
      bind_builtin( "LEAVES" ,
                    builtin_flags, T_FLAG_LEAVES, 0 ) );

    duplicate_rule( "Match" ,
      bind_builtin( "MATCH" ,
                    builtin_match, 0, 0 ) );

    duplicate_rule( "NoCare" ,
      bind_builtin( "NOCARE" ,
                    builtin_flags, T_FLAG_NOCARE, 0 ) );

    duplicate_rule( "NOTIME" ,
    duplicate_rule( "NotFile" ,
      bind_builtin( "NOTFILE" ,
                    builtin_flags, T_FLAG_NOTFILE, 0 ) ) );

    duplicate_rule( "NoUpdate" ,
      bind_builtin( "NOUPDATE" ,
                    builtin_flags, T_FLAG_NOUPDATE, 0 ) );

    duplicate_rule( "Temporary" ,
      bind_builtin( "TEMPORARY" ,
                    builtin_flags, T_FLAG_TEMP, 0 ) );

    {
        char * args[] = { "targets", "*", 0 };
        bind_builtin(
            "ISFILE",
            builtin_flags, T_FLAG_ISFILE, 0 );
    }

    duplicate_rule( "HdrMacro" ,
      bind_builtin( "HDRMACRO" ,
                    builtin_hdrmacro, 0, 0 ) );

    /* FAIL_EXPECTED is used to indicate that the result of a target build */
    /* action should be inverted (ok <=> fail) this can be useful when     */
    /* performing test runs from Jamfiles..                                */
      bind_builtin( "FAIL_EXPECTED" ,
                    builtin_flags, T_FLAG_FAIL_EXPECTED, 0 );

      bind_builtin( "RMOLD" , builtin_flags, T_FLAG_RMOLD, 0 );
      
      {
          char * args[] = { "targets", "*", 0 };
          bind_builtin( "UPDATE", builtin_update, 0, args );
      }

      {
          char * args[] = { "string", "pattern", "replacements", "+", 0 };
          duplicate_rule( "subst" ,
            bind_builtin( "SUBST" ,
                          builtin_subst, 0, args ) );
      }

      {
          char * args[] = { "module", "?", 0 };
          bind_builtin( "RULENAMES" ,
                         builtin_rulenames, 0, args );
      }


      {
          char * args[] = { "module", "?", 0 };
          bind_builtin( "VARNAMES" ,
                         builtin_varnames, 0, args );
      }

      {
          char * args[] = { "module", "?", 0 };
          bind_builtin( "DELETE_MODULE" ,
                         builtin_delete_module, 0, args );
      }

      {
           char * args[] = { "source_module", "?",
                             ":", "source_rules", "*",
                             ":", "target_module", "?",
                             ":", "target_rules", "*",
                             ":", "localize", "?", 0 };
           bind_builtin( "IMPORT" ,
                         builtin_import, 0, args );
      }

      {
          char * args[] = { "module", "?", ":", "rules", "*", 0 };
          bind_builtin( "EXPORT" ,
                        builtin_export, 0, args );
      }

      {
          char * args[] = { "levels", "?", 0 };
          bind_builtin( "CALLER_MODULE" ,
                         builtin_caller_module, 0, args );
      }

      {
          char * args[] = { "levels", "?", 0 };
          bind_builtin( "BACKTRACE" ,
                        builtin_backtrace, 0, args );
      }

      {
          char * args[] = { 0 };
          bind_builtin( "PWD" ,
                        builtin_pwd, 0, args );
      }

      {
          char * args[] = { "target", "*", ":", "path", "*", 0 };
          bind_builtin( "SEARCH_FOR_TARGET",
                        builtin_search_for_target, 0, args );
      }

      {
          char * args[] = { "modules_to_import", "+", ":", "target_module", "?", 0 };
          bind_builtin( "IMPORT_MODULE",
                        builtin_import_module, 0, args );
      }

      {
          char * args[] = { "module", "?", 0 };
          bind_builtin( "IMPORTED_MODULES",
                        builtin_imported_modules, 0, args );
      }

      {
          char * args[] = { "instance_module", ":", "class_module", 0 };
          bind_builtin( "INSTANCE",
                        builtin_instance, 0, args );
      }

      {
          char * args[] = { "sequence", "*", 0 };
          bind_builtin( "SORT",
                        builtin_sort, 0, args );
      }

      {
          char * args[] = { "path_parts", "*", 0 };
          bind_builtin( "NORMALIZE_PATH",
              builtin_normalize_path, 0, args );
      }

      {
          char * args[] = { "args", "*", 0 };
          bind_builtin( "CALC",
              builtin_calc, 0, args );
      }

      {
          char * args[] = { "module", ":", "rule", 0 };
          bind_builtin( "NATIVE_RULE",
              builtin_native_rule, 0, args );
      }

      {
          char * args[] = { "module", ":", "rule", ":", "version", 0 };
          bind_builtin( "HAS_NATIVE_RULE",
              builtin_has_native_rule, 0, args );
      }


      {
          char * args[] = { "module", "*", 0 };
          bind_builtin( "USER_MODULE",
              builtin_user_module, 0, args );
      }

      {
          char * args[] = { 0 };
          bind_builtin( "NEAREST_USER_LOCATION",
              builtin_nearest_user_location, 0, args );
      }

      {
          char * args[] = { "file", 0 };
          bind_builtin( "CHECK_IF_FILE",
                        builtin_check_if_file, 0, args );
      }

#ifdef HAVE_PYTHON
      {
          char * args[] = { "python-module", ":", "function", ":", 
                            "jam-module", ":", "rule-name", 0 };
          bind_builtin( "PYTHON_IMPORT_RULE",
              builtin_python_import_rule, 0, args );
      }
#endif

# if defined( OS_NT ) || defined( OS_CYGWIN )
      {
          char * args[] = { "key_path", ":", "data", "?", 0 };
          bind_builtin( "W32_GETREG",
              builtin_system_registry, 0, args );
      }

      {
          char * args[] = { "key_path", ":", "result-type", 0 };
          bind_builtin( "W32_GETREGNAMES",
              builtin_system_registry_names, 0, args );
      }
# endif

      {
          char * args[] = { "command", ":", "*", 0 };
          bind_builtin( "SHELL",
              builtin_shell, 0, args );
          bind_builtin( "COMMAND",
              builtin_shell, 0, args );
      }

      /* Initialize builtin modules */
      init_set();
      init_path();
      init_regex();
      init_property_set();
      init_sequence();
      init_order();
}

/*
* builtin_calc() - CALC rule
*
* The CALC rule performs simple mathematical operations on two arguments.
*/

LIST *
builtin_calc(
    PARSE *parse,
    FRAME *frame )
{
    LIST *arg = lol_get( frame->args, 0 );

    LIST *result = 0;
    long lhs_value;
    long rhs_value;
    long result_value;
    char buffer [16];
    const char* lhs;
    const char* op;
    const char* rhs;

    if (arg == 0) return L0;
    lhs = arg->string;

    arg = list_next( arg );
    if (arg == 0) return L0;
    op = arg->string;

    arg = list_next( arg );
    if (arg == 0) return L0;
    rhs = arg->string;

    lhs_value = atoi (lhs);
    rhs_value = atoi (rhs);

    if (strcmp ("+", op) == 0)
    {
        result_value = lhs_value + rhs_value;
    }
    else if (strcmp ("-", op) == 0)
    {
        result_value = lhs_value - rhs_value;
    }
    else
    {
        return L0;
    }

    sprintf (buffer, "%ld", result_value);
    result = list_new( result, newstr( buffer ) );
    return result;
}

/*
 * builtin_depends() - DEPENDS/INCLUDES rule
 *
 * The DEPENDS builtin rule appends each of the listed sources on the 
 * dependency list of each of the listed targets.  It binds both the 
 * targets and sources as TARGETs.
 */

LIST *
builtin_depends(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *targets = lol_get( frame->args, 0 );
	LIST *sources = lol_get( frame->args, 1 );
	LIST *l;

	for( l = targets; l; l = list_next( l ) )
	{
	    TARGET *t = bindtarget( l->string );

	    /* If doing INCLUDES, switch to the TARGET's include */
	    /* TARGET, creating it if needed.  The internal include */
	    /* TARGET shares the name of its parent. */

	    if( parse->num )
	    {
            if( !t->includes ) {
                t->includes = copytarget( t );
                t->includes->original_target = t;
            }
            t = t->includes;
	    }

	    t->depends = targetlist( t->depends, sources );
	}

    /* Enter reverse links */
	for( l = sources; l; l = list_next( l ) )
	{
	    TARGET *s = bindtarget( l->string );
        s->dependents = targetlist( s->dependents, targets );
    }

	return L0;
}

/*
 * builtin_rebuilds() - REBUILDS rule
 *
 * The REBUILDS builtin rule appends each of the listed
 * rebuild-targets in its 2nd argument on the rebuilds list of each of
 * the listed targets in its first argument.
 */

LIST *
builtin_rebuilds(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *targets = lol_get( frame->args, 0 );
	LIST *rebuilds = lol_get( frame->args, 1 );
	LIST *l;

	for( l = targets; l; l = list_next( l ) )
	{
	    TARGET *t = bindtarget( l->string );
	    t->rebuilds = targetlist( t->rebuilds, rebuilds );
	}

	return L0;
}

/*
 * builtin_echo() - ECHO rule
 *
 * The ECHO builtin rule echoes the targets to the user.  No other 
 * actions are taken.
 */

LIST *
builtin_echo(
	PARSE	*parse,
	FRAME *frame )
{
	list_print( lol_get( frame->args, 0 ) );
	printf( "\n" );
	return L0;
}

/*
 * builtin_exit() - EXIT rule
 *
 * The EXIT builtin rule echoes the targets to the user and exits
 * the program with a failure status.
 */

LIST *
builtin_exit(
    PARSE   *parse,
    FRAME   *frame )
{
    list_print( lol_get( frame->args, 0 ) );
    printf( "\n" );
    if ( lol_get( frame->args, 1 ) )
    {
        exit ( atoi( lol_get( frame->args, 1 )->string ) );
    }
    else
    {
        exit( EXITBAD ); /* yeech */
    }
    return L0;
}

/*
 * builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *
 * Builtin_flags() marks the target with the appropriate flag, for use
 * by make0().  It binds each target as a TARGET.
 */

LIST *
builtin_flags(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *l = lol_get( frame->args, 0 );

	for( ; l; l = list_next( l ) )
	    bindtarget( l->string )->flags |= parse->num;

	return L0;
}

/*
 * builtin_globbing() - GLOB rule
 */

struct globbing {
    LIST    *patterns;
    LIST    *results;
    LIST    *case_insensitive;
} ;

static void downcase_inplace( char* p )
{
    for ( ; *p; ++p )
    {
        *p = tolower(*p);
    }
}
    
static void
builtin_glob_back(
    void    *closure,
    char    *file,
    int status,
    time_t  time )
{
    PROFILE_ENTER(BUILTIN_GLOB_BACK);
    
    struct globbing *globbing = (struct globbing *)closure;
    LIST        *l;
    PATHNAME    f;
    string          buf[1];
    
    /* Null out directory for matching. */
    /* We wish we had file_dirscan() pass up a PATHNAME. */

    path_parse( file, &f );
    f.f_dir.len = 0;

    /* For globbing, we unconditionally ignore current and parent
       directory items. Since they items always exist, there's not
       reason why caller of GLOB would want to see them.
       We could also change file_dirscan, but then paths with embedded
       "." and ".." won't work anywhere.
    */
    if (strcmp(f.f_base.ptr, ".") == 0 || strcmp(f.f_base.ptr, "..") == 0)
    {
        PROFILE_EXIT(BUILTIN_GLOB_BACK);
        return;
    }

    string_new( buf );
    path_build( &f, buf, 0 );

    if (globbing->case_insensitive) {
        downcase_inplace( buf->value );
    }

    for( l = globbing->patterns; l; l = l->next )
    {
        if( !glob( l->string, buf->value ) )
        {
            globbing->results = list_new( globbing->results, newstr( file ) );
            break;
        }
    }
    
    string_free( buf );
    
    PROFILE_EXIT(BUILTIN_GLOB_BACK);
}

static LIST* downcase_list( LIST *in )
{
    LIST* result = 0;
    
    string s[1];
    string_new( s );
        
    while (in)
    {
        string_copy( s, in->string );
        downcase_inplace( s->value );
        result = list_append( result, list_new( 0, newstr( s->value ) ) );
        in = in->next;
    }
    
    string_free( s );
    return result;
}

LIST *
builtin_glob(
    PARSE   *parse,
    FRAME *frame )
{
    LIST *l = lol_get( frame->args, 0 );
    LIST *r = lol_get( frame->args, 1 );
    
    struct globbing globbing;

    globbing.results = L0;
    globbing.patterns = r;
    
    globbing.case_insensitive
# if defined( OS_NT ) || defined( OS_CYGWIN )
       = l;  /* always case-insensitive if any files can be found */
# else 
       = lol_get( frame->args, 2 );
# endif

    if ( globbing.case_insensitive )
    {
        globbing.patterns = downcase_list( r );
    }
    
    for( ; l; l = list_next( l ) )
        file_dirscan( l->string, builtin_glob_back, &globbing );

    if ( globbing.case_insensitive )
    {
        list_free( globbing.patterns );
    }
    return globbing.results;
}

static int has_wildcards(const char* str)
{
    size_t index = strcspn(str, "[]*?");
    if (str[index] == '\0')
        return 0;
    else
        return 1;
}

/** If 'file' exists, append 'file' to 'list'.
    Returns 'list'.
*/
static LIST* append_if_exists(LIST* list, char* file)
{
    time_t time;
    timestamp(file, &time);
    if (time > 0)
        return list_new(list, newstr(file));
    else
        return list;        
}

LIST* glob1(char* dirname, char* pattern)
{
    LIST* plist = list_new(L0, pattern);
    struct globbing globbing;

    globbing.results = L0;
    globbing.patterns = plist;
    
    globbing.case_insensitive
# if defined( OS_NT ) || defined( OS_CYGWIN )
       = plist;  /* always case-insensitive if any files can be found */
# else 
       = L0;
# endif

    if ( globbing.case_insensitive )
    {
        globbing.patterns = downcase_list( plist );
    }
    
    file_dirscan( dirname, builtin_glob_back, &globbing );

    if ( globbing.case_insensitive )
    {
        list_free( globbing.patterns );
    }

    list_free(plist);

    return globbing.results;
}


LIST* glob_recursive(char* pattern)
{
    LIST* result = L0;

    /* Check if there's metacharacters in pattern */
    if (!has_wildcards(pattern))
    {
        /* No metacharacters. Check if the path exists. */
        result = append_if_exists(result, pattern);
    }        
    else
    {
        /* Have metacharacters in the pattern. Split into dir/name */
        PATHNAME path[1];
        path_parse(pattern, path);            
        
        if (path->f_dir.ptr)
        {
            LIST* dirs = L0;
            string dirname[1];
            string basename[1];
            string_new(dirname);
            string_new(basename);

            string_append_range(dirname, path->f_dir.ptr, 
                                path->f_dir.ptr + path->f_dir.len);

            path->f_grist.ptr = 0;
            path->f_grist.len = 0;
            path->f_dir.ptr = 0;
            path->f_dir.len = 0;
            path_build(path, basename, 0);

            if (has_wildcards(dirname->value))
            {
                dirs = glob_recursive(dirname->value);
            }
            else
            {
                dirs = list_new(dirs, dirname->value);
            }
            
            if (has_wildcards(basename->value))
            {
                for(; dirs; dirs = dirs->next)
                {
                    result = list_append(result, 
                                         glob1(dirs->string, basename->value));
                }
            }
            else
            {
                string file_string[1];
                string_new(file_string);

                /** No wildcard in basename. */
                for(; dirs; dirs = dirs->next)
                {                                      
                    path->f_dir.ptr = dirs->string;
                    path->f_dir.len = strlen(dirs->string);                    
                    path_build(path, file_string, 0);

                    result = append_if_exists(result, file_string->value);

                    string_truncate(file_string, 0);
                }

                string_free(file_string);
            }

            string_free(dirname);
            string_free(basename);
        }
        else
        {
            /** No directory, just a pattern. */
            result = list_append(result, glob1(".", pattern));
        }
    }

    return result;
}

LIST *
builtin_glob_recursive(
    PARSE   *parse,
    FRAME *frame )
{
    LIST* result = L0;
    LIST* l = lol_get( frame->args, 0 );

    for(; l; l = l->next)
    {
        result = list_append(result, glob_recursive(l->string));
    }

    return result;
}

/*
 * builtin_match() - MATCH rule, regexp matching
 */

LIST *
builtin_match(
	PARSE	*parse,
	FRAME	*frame )
{
	LIST *l, *r;
	LIST *result = 0;
        
        string buf[1];
        string_new(buf);

	/* For each pattern */

	for( l = lol_get( frame->args, 0 ); l; l = l->next )
	{
            /* Result is cached and intentionally never freed */
	    regexp *re = regex_compile( l->string );

	    /* For each string to match against */
            for( r = lol_get( frame->args, 1 ); r; r = r->next )
            {
                if( regexec( re, r->string ) )
                {
                    int i, top;

                    /* Find highest parameter */

                    for( top = NSUBEXP; top-- > 1; )
                        if( re->startp[top] )
                            break;

                    /* And add all parameters up to highest onto list. */
                    /* Must have parameters to have results! */

                    for( i = 1; i <= top; i++ )
                    {
                        string_append_range( buf, re->startp[i], re->endp[i] );
                        result = list_new( result, newstr( buf->value ) );
                        string_truncate( buf, 0 );
                    }
                }
            }
        }

        string_free( buf );
        return result;
}

LIST *
builtin_hdrmacro(
    PARSE    *parse,
    FRAME *frame )
{
  LIST*  l = lol_get( frame->args, 0 );
  
  for ( ; l; l = list_next(l) )
  {
    TARGET*  t = bindtarget( l->string );

    /* scan file for header filename macro definitions */    
    if ( DEBUG_HEADER )
      printf( "scanning '%s' for header file macro definitions\n",
              l->string );

    macro_headers( t );
  }
  
  return L0;
}

/*  builtin_rulenames() - RULENAMES ( MODULE ? )
 *
 *  Returns a list of the non-local rule names in the given MODULE. If
 *  MODULE is not supplied, returns the list of rule names in the
 *  global module.
 */

/* helper function for builtin_rulenames(), below */
static void add_rule_name( void* r_, void* result_ )
{
    RULE* r = (RULE*)r_;
    LIST** result = (LIST**)result_;

    if ( r->exported )
        *result = list_new( *result, copystr( r->name ) );
}

LIST *
builtin_rulenames(
    PARSE   *parse,
    FRAME *frame )
{
    LIST *arg0 = lol_get( frame->args, 0 );
    LIST *result = L0;
    module_t* source_module = bindmodule( arg0 ? arg0->string : 0 );

    if ( source_module->rules )
        hashenumerate( source_module->rules, add_rule_name, &result );
    return result;
}

/*  builtin_varnames() - VARNAMES ( MODULE ? )
 *
 *  Returns a list of the variable names in the given MODULE. If
 *  MODULE is not supplied, returns the list of variable names in the
 *  global module.
 */

/* helper function for builtin_varnames(), below.  Used with
 * hashenumerate, will prepend the key of each element to a list
 */
static void add_hash_key( void* np, void* result_ )
{
    LIST** result = (LIST**)result_;

    *result = list_new( *result, copystr( *(char**)np ) );
}

static struct hash *get_running_module_vars()
{
    struct hash *dummy, *vars = NULL;
    /* Get the global variables pointer (that of the currently running module) */
    var_hash_swap(&vars);
    dummy = vars;
    /* Put the global variables pointer in its right place */
    var_hash_swap(&dummy);
    return vars;
}

LIST *
builtin_varnames(
    PARSE   *parse,
    FRAME *frame )
{
    LIST *arg0 = lol_get( frame->args, 0 );
    LIST *result = L0;
    module_t* source_module = bindmodule( arg0 ? arg0->string : 0 );

    /* The running module _always_ has its 'variables' member set to NULL
     * due to the way enter_module and var_hash_swap work */
    struct hash *vars = 
        source_module == frame->module ? 
            get_running_module_vars() : source_module->variables;

    if ( vars )
        hashenumerate( vars, add_hash_key, &result );
    return result;
}

/*
 * builtin_delete_module() - MODULE ?
 *
 * Clears all rules and variables from the given module.
 */
LIST *
builtin_delete_module(
    PARSE   *parse,
    FRAME *frame )
{
    LIST *arg0 = lol_get( frame->args, 0 );
    LIST *result = L0;
    module_t* source_module = bindmodule( arg0 ? arg0->string : 0 );

    delete_module( source_module );
    return result;
}

static void unknown_rule( FRAME *frame, char* key, char *module_name, char *rule_name )
{
    backtrace_line( frame->prev );
    printf( "%s error: rule \"%s\" unknown in module \"%s\"\n", key, rule_name, module_name );
    backtrace( frame->prev );
    exit(1);
    
}

/*
 * builtin_import() - IMPORT ( SOURCE_MODULE ? : SOURCE_RULES * : TARGET_MODULE ? : TARGET_RULES * : LOCALIZE ? )
 *
 * The IMPORT rule imports rules from the SOURCE_MODULE into the
 * TARGET_MODULE as local rules. If either SOURCE_MODULE or
 * TARGET_MODULE is not supplied, it refers to the global
 * module. SOURCE_RULES specifies which rules from the SOURCE_MODULE
 * to import; TARGET_RULES specifies the names to give those rules in
 * TARGET_MODULE. If SOURCE_RULES contains a name which doesn't
 * correspond to a rule in SOURCE_MODULE, or if it contains a
 * different number of items than TARGET_RULES, an error is issued.
 * if LOCALIZE is specified, the rules will be executed in
 * TARGET_MODULE, with corresponding access to its module local
 * variables.
 */
LIST *
builtin_import(
    PARSE *parse,
    FRAME *frame )
{
    LIST *source_module_list = lol_get( frame->args, 0 );
    LIST *source_rules = lol_get( frame->args, 1 );
    LIST *target_module_list = lol_get( frame->args, 2 );
    LIST *target_rules = lol_get( frame->args, 3 );
    LIST *localize = lol_get( frame->args, 4 );

    module_t* target_module = bindmodule( target_module_list ? target_module_list->string : 0 );
    module_t* source_module = bindmodule( source_module_list ? source_module_list->string : 0 );
    
    LIST *source_name, *target_name;
            
    for ( source_name = source_rules, target_name = target_rules;
          source_name && target_name;
          source_name = list_next( source_name )
          , target_name = list_next( target_name ) )
    {
        RULE r_, *r = &r_, *imported;
        r_.name = source_name->string;
                
        if ( !source_module->rules
             || !hashcheck( source_module->rules, (HASHDATA**)&r )
            )
        {
            unknown_rule( frame, "IMPORT", source_module->name, r_.name );
        }
        
        imported = import_rule( r, target_module, target_name->string );
        if ( localize )
            imported->module = target_module;
        imported->exported = 0; /* this rule is really part of some other module; just refer to it here, but don't let it out */
    }
    
    if ( source_name || target_name )
    {
        backtrace_line( frame->prev );
        printf( "import error: length of source and target rule name lists don't match!\n" );
        printf( "    source: " );
        list_print( source_rules );
        printf( "\n    target: " );
        list_print( target_rules );
        printf( "\n" );
        backtrace( frame->prev );
        exit(1);
    }

    return L0;
}


/*
 * builtin_export() - EXPORT ( MODULE ? : RULES * )
 *
 * The EXPORT rule marks RULES from the SOURCE_MODULE as non-local
 * (and thus exportable). If an element of RULES does not name a rule
 * in MODULE, an error is issued.
 */
LIST *
builtin_export(
    PARSE *parse,
    FRAME *frame )
{
    LIST *module_list = lol_get( frame->args, 0 );
    LIST *rules = lol_get( frame->args, 1 );

    module_t* m = bindmodule( module_list ? module_list->string : 0 );
    
            
    for ( ; rules; rules = list_next( rules ) )
    {
        RULE r_, *r = &r_;
        r_.name = rules->string;
                
        if ( !m->rules || !hashcheck( m->rules, (HASHDATA**)&r ) )
            unknown_rule( frame, "EXPORT", m->name, r_.name );
        
        r->exported = 1;
    }
    return L0;
}

/*  Retrieve the file and line number that should be indicated for a
 *  given procedure in debug output or an error backtrace
 */
static void get_source_line( PARSE* procedure, char** file, int* line )
{
    if ( procedure )
    {
        char* f = procedure->file;
        int l = procedure->line;
        if ( !strcmp( f, "+" ) )
        {
            f = "jambase.c";
            l += 3;
        }
        *file = f;
        *line = l;
    }
    else
    {
        *file = "(builtin)";
        *line = -1;
    }
}

void print_source_line( PARSE* p )
{
    char* file;
    int line;

    get_source_line( p, &file, &line );
    if ( line < 0 )
        printf( "(builtin):" );
    else
        printf( "%s:%d:", file, line);
}

/* Print a single line of error backtrace for the given frame */
void backtrace_line( FRAME *frame )
{
    if ( frame == 0 )
    {
        printf( "(no frame):" );
    }
    else
    {
        print_source_line( frame->procedure );
        printf( " in %s\n", frame->rulename );
    }
}

/*  Print the entire backtrace from the given frame to the Jambase
 *  which invoked it.
 */
void backtrace( FRAME *frame )
{
	if ( !frame ) return;
    while ( frame = frame->prev )
    {
        backtrace_line( frame );
    }
}

/*  A Jam version of the backtrace function, taking no arguments and
 *  returning a list of quadruples: FILENAME LINE MODULE. RULENAME
 *  describing each frame. Note that the module-name is always
 *  followed by a period.
 */
LIST *builtin_backtrace( PARSE *parse, FRAME *frame )
{
    LIST* levels_arg = lol_get( frame->args, 0 );
    int levels = levels_arg ? atoi( levels_arg->string ) : ((unsigned int)(-1) >> 1) ;

    LIST* result = L0;
    for(; (frame = frame->prev) && levels ; --levels )
    {
        char* file;
        int line;
        char buf[32];
        get_source_line( frame->procedure, &file, &line );
        sprintf( buf, "%d", line );
        result = list_new( result, newstr( file ) );
        result = list_new( result, newstr( buf ) );
        result = list_new( result, newstr( frame->module->name ) );
        result = list_new( result, newstr( frame->rulename ) );
    }
    return result;
}

/*
 * builtin_caller_module() - CALLER_MODULE ( levels ? )
 *
 * If levels is not supplied, returns the name of the module of the rule which
 * called the one calling this one. If levels is supplied, it is interpreted as
 * an integer specifying a number of additional levels of call stack to traverse
 * in order to locate the module in question. If no such module exists,
 * returns the empty list. Also returns the empty list when the module in
 * question is the global module. This rule is needed for implementing module
 * import behavior.
 */
LIST *builtin_caller_module( PARSE *parse, FRAME *frame )
{
    LIST* levels_arg = lol_get( frame->args, 0 );
    int levels = levels_arg ? atoi( levels_arg->string ) : 0 ;

    int i;
    for (i = 0; i < levels + 2 && frame->prev; ++i)
        frame = frame->prev;

    if ( frame->module == root_module() )
    {
        return L0;
    }
    else
    {
        LIST* result;
        
        string name;
        string_copy( &name, frame->module->name );
        string_pop_back( &name );

        result = list_new( L0, newstr(name.value) );
        
        string_free( &name );
        
        return result;
    }
}

/*
 * Return the current working directory.
 *
 * Usage: pwd = [ PWD ] ;
 */
LIST*
builtin_pwd( PARSE *parse, FRAME *frame )
{
    return pwd();
}

/*
 * Adds targets to the list of target that jam will attempt to update.
 */
LIST* 
builtin_update( PARSE *parse, FRAME *frame)
{
    LIST* result = list_copy( L0, targets_to_update() );
    LIST* arg1 = lol_get( frame->args, 0 );
    clear_targets_to_update();
    for ( ; arg1; arg1 = list_next( arg1 ) )
        mark_target_for_updating( newstr(arg1->string) );
    return result;
}

LIST*
builtin_search_for_target( PARSE *parse, FRAME *frame )
{
    LIST* arg1 = lol_get( frame->args, 0 );
    LIST* arg2 = lol_get( frame->args, 1 );

    TARGET* t = search_for_target( arg1->string, arg2 );
    return list_new( L0, t->name );
}

LIST *builtin_import_module( PARSE *parse, FRAME *frame )
{
    LIST* arg1 = lol_get( frame->args, 0 );
    LIST* arg2 = lol_get( frame->args, 1 );

    module_t* m = arg2 ? bindmodule(arg2->string) : root_module();

    import_module(arg1, m);

    return L0;
}


LIST *builtin_imported_modules( PARSE *parse, FRAME *frame )
{
    LIST *arg0 = lol_get( frame->args, 0 );
    module_t* source_module = bindmodule( arg0 ? arg0->string : 0 );

    return imported_modules(source_module);
}

LIST *builtin_instance( PARSE *parse, FRAME *frame )
{
    LIST* arg1 = lol_get( frame->args, 0 );
    LIST* arg2 = lol_get( frame->args, 1 );

    module_t* instance = bindmodule( arg1->string );
    module_t* class_module = bindmodule( arg2->string );
    instance->class_module = class_module;

    return L0;
}

LIST*
builtin_sort( PARSE *parse, FRAME *frame )
{
    LIST* arg1 = lol_get( frame->args, 0 );

    return list_sort(arg1);
}

LIST *builtin_normalize_path( PARSE *parse, FRAME *frame )
{
    LIST* arg = lol_get( frame->args, 0 );

    /* First, we iterate over all '/'-separated elements, starting from
       the end of string. If we see '..', we remove previous path elements.
       If we see '.', we remove it.
       The removal is done by putting '\1' in the string. After all the string
       is processed, we do a second pass, removing '\1' characters.
    */
    
    string in[1], out[1], tmp[1];
    char* end;      /* Last character of the part of string still to be processed. */
    char* current;  /* Working pointer. */  
    int dotdots = 0; /* Number of '..' elements seen and not processed yet. */
    int rooted = arg->string[0] == '/';
    char* result;

    /* Make a copy of input: we should not change it. */
    string_new(in);
    if (!rooted)
        string_push_back(in, '/');
    while (arg)
    {
        string_append(in, arg->string);
        arg = list_next(arg);
        if (arg)
            string_append(in, "/");
    }
    

    end = in->value + in->size - 1;
    current = end;
    
    for(;end >= in->value;) {
        /* Set 'current' to the next occurence of '/', which always exists. */
        for(current = end; *current != '/'; --current)
            ;
        
        if (current == end && current != in->value) {
            /* Found a trailing slash. Remove it. */
            *current = '\1';
        } else if (current == end && *(current+1) == '/') {
            /* Found duplicated slash. Remove it. */
            *current = '\1';
        } else if (end - current == 1 && strncmp(current, "/.", 2) == 0) {
            /* Found '/.'. Drop them all. */
            *current = '\1';
            *(current+1) = '\1';                   
        } else if (end - current == 2 && strncmp(current, "/..", 3) == 0) {
            /* Found '/..' */                
            *current = '\1';
            *(current+1) = '\1';                   
            *(current+2) = '\1';                   
            ++dotdots;
        } else if (dotdots) {
            char* p = current;
            memset(current, '\1', end-current+1);
            --dotdots;
        }                 
        end = current-1;
    }


    string_new(tmp);
    while(dotdots--)
        string_append(tmp, "/..");
    string_append(tmp, in->value);
    string_copy(in, tmp->value);
    string_free(tmp);
        
       
    string_new(out);
    /* The resulting path is either empty or has '/' as the first significant
       element. If the original path was not rooted, we need to drop first '/'. 
       If the original path was rooted, and we've got empty path, need to add '/'
    */
    if (!rooted) {
        current = strchr(in->value, '/');
        if (current)
            *current = '\1';
    } 
       
    for (current = in->value; *current; ++current)
        if (*current != '\1')
            string_push_back(out, *current);

    
    result = newstr(out->size ? out->value : (rooted ? "/" : "."));
    string_free(in);
    string_free(out);

    return list_new(0, result);

}

LIST *builtin_native_rule( PARSE *parse, FRAME *frame )
{
    LIST* module_name = lol_get( frame->args, 0 );    
    LIST* rule_name = lol_get( frame->args, 1 );    

    module_t* module = bindmodule(module_name->string);

    native_rule_t n, *np = &n;
    n.name = rule_name->string;
    if (module->native_rules && hashcheck(module->native_rules, (HASHDATA**)&np))
    {
        new_rule_body(module, np->name, np->arguments, np->procedure, 1);
    }
    else
    {
        backtrace_line( frame->prev );
        printf( "error: no native rule \"%s\" defined in module \"%s\"\n", 
                n.name, module->name);
        backtrace( frame->prev );
        exit(1);
    }
    return L0;    
}

LIST *builtin_has_native_rule( PARSE *parse, FRAME *frame )
{
    LIST* module_name = lol_get( frame->args, 0 );    
    LIST* rule_name = lol_get( frame->args, 1 );    
    LIST* version = lol_get( frame->args, 2 );    

    module_t* module = bindmodule(module_name->string);

    native_rule_t n, *np = &n;
    n.name = rule_name->string;
    if (module->native_rules && hashcheck(module->native_rules, (HASHDATA**)&np))
    {
        int expected_version = atoi(version->string);
        if (np->version == expected_version)
            return list_new(0, newstr("true"));
    }
    return L0;    
}


LIST *builtin_user_module( PARSE *parse, FRAME *frame )
{
    LIST* module_name = lol_get( frame->args, 0 );    
    for(; module_name; module_name = module_name->next) 
    {
        module_t* m = bindmodule( module_name->string);
        m->user_module = 1;
    }
    return L0;
}

LIST *builtin_nearest_user_location( PARSE *parse, FRAME *frame )
{
    LIST* result = 0;
    FRAME* nearest_user_frame = 
        frame->module->user_module ? frame : frame->prev_user;

    if (nearest_user_frame)
    {
        char* file;
        int line;
        char buf[32];
        get_source_line( nearest_user_frame->procedure, &file, &line );
        sprintf( buf, "%d", line );
        result = list_new( result, newstr( file ) );
        result = list_new( result, newstr( buf ) );
        return result;
    }
    else
    {
        return L0;
    }
}

LIST *builtin_check_if_file( PARSE *parse, FRAME *frame )
{
    LIST* name = lol_get( frame->args, 0 );
    if (file_is_file(name->string) == 1) {
        return list_new(0, newstr("true"));
    } else {
        return L0;
    }
}


#ifdef HAVE_PYTHON

LIST *builtin_python_import_rule( PARSE *parse, FRAME *frame )
{
    static int first_time = 1;
   char* python_module = lol_get( frame->args, 0 )->string;        
   char* python_function = lol_get( frame->args, 1 )->string;        
   char* jam_module = lol_get( frame->args, 2 )->string;        
   char* jam_rule = lol_get( frame->args, 3 )->string;        

   PyObject *pName, *pModule, *pDict, *pFunc;

   if (first_time)
   {
       /* At the first invocation, we add the value of the
          global EXTRA_PYTHONPATH to the sys.path Python
          variable.
       */
       LIST* extra = 0;
       module_t* outer_module = frame->module;

       first_time = 0;

       if ( outer_module != root_module())
       {
           exit_module( outer_module );
           enter_module( root_module());
       }
    
       extra = var_get("EXTRA_PYTHONPATH");
    
       if ( outer_module != root_module())
       {
            exit_module( root_module());
            enter_module( outer_module );
       }

       for(; extra; extra = extra->next)
       {
           string buf[1];
           string_new(buf);
           string_append(buf, "import sys\nsys.path.append(\"");
           string_append(buf, extra->string);
           string_append(buf, "\")\n");
           PyRun_SimpleString(buf->value);   
           string_free(buf);               
       }       
   }


   pName = PyString_FromString(python_module);
   
   pModule = PyImport_Import(pName);
   Py_DECREF(pName);

   if (pModule != NULL) {
        pDict = PyModule_GetDict(pModule);
        pFunc = PyDict_GetItemString(pDict, python_function);

        if (pFunc && PyCallable_Check(pFunc)) {

            module_t* m = bindmodule(jam_module);
            RULE* r = bindrule( jam_rule, m );

            /* Make pFunc owned */
            Py_INCREF(pFunc);

            r->python_function = pFunc;
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", python_function);
        }
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", python_module);
    }
   return L0;

}

#endif

void lol_build( LOL* lol, char** elements )
{
    LIST* l = L0;
    lol_init( lol );
    
    while ( elements && *elements )
    {
        if ( !strcmp( *elements, ":" ) )
        {
            lol_add( lol, l );
            l = L0 ;
        }
        else
        {
            l = list_new( l, newstr( *elements ) );
        }
        ++elements;
    }
    
    if ( l != L0 )
        lol_add( lol, l );
}

#ifdef HAVE_PYTHON

/** Calls the bjam rule specified by name passed in 'args'.
    The name is looked up in context of bjam's 'python_interface'
    module. Returns the list of string retured by the rule.
*/
PyObject*
bjam_call(PyObject* self, PyObject* args)
{
    FRAME       inner[1];
    LIST    *result;
    PARSE   *p;
    char*  rulename;
    
    /* Build up the list of arg lists */

    frame_init( inner );
    inner->prev = 0;
    inner->prev_user = 0;
    inner->module = bindmodule("python_interface");
    inner->procedure = 0;

    /* Extract the rule name and arguments from 'args' */

    /* PyTuple_GetItem returns borrowed reference */
    rulename = PyString_AsString(PyTuple_GetItem(args, 0));
    {
        int i = 1;
        int size = PyTuple_Size(args);
        for( ; i < size; ++i) {
            PyObject* a = PyTuple_GetItem(args, i);
            if (PyString_Check(a))
            {
                lol_add(inner->args, 
                        list_new(0, newstr(PyString_AsString(a))));
            }
            else if (PySequence_Check(a))
            {
                LIST* l = 0;
                int s = PySequence_Size(a);
                int i = 0;
                for(; i < s; ++i)
                {
                    /* PySequence_GetItem returns new reference. */
                    PyObject* e = PySequence_GetItem(a, i);
                    char* s = PyString_AsString(e);
                    if (!s)
                    {
                        printf("Invalid parameter type passed from Python\n");
                        exit(1);
                    }
                    l = list_new(l, newstr(s));
                    Py_DECREF(e);
                }
                lol_add(inner->args, l);
            }                
        }
    }

    result = evaluate_rule( rulename, inner );

    frame_free( inner );

    return Py_None;
}

/** Accepts three arguments: module name, rule name and Python callable.

    Creates bjam rule with the specified name in the specified module,
    which will invoke the Python callable.
*/
PyObject*
bjam_import_rule(PyObject* self, PyObject* args)
{
    char* module;
    char* rule;
    PyObject* func;
    module_t* m;
    RULE* r;

    if (!PyArg_ParseTuple(args, "ssO:import_rule", &module, &rule, &func))
        return NULL;
    
    if (!PyCallable_Check(func))
        return NULL;
    
    m = bindmodule(module);
    r = bindrule(rule, m);

    /* Make pFunc owned */
    Py_INCREF(func);

    r->python_function = func;
    return Py_None;
}

#endif

#ifdef HAVE_POPEN
#if defined(_MSC_VER) || defined(__BORLANDC__)
    #define popen _popen
    #define pclose _pclose
#endif

LIST *builtin_shell( PARSE *parse, FRAME *frame )
{
    LIST* command = lol_get( frame->args, 0 );
    LIST* result = 0;
    string s;
    int ret;
    char buffer[1024];
    FILE *p = NULL;
    int exit_status = -1;
    int exit_status_opt = 0;
    int no_output_opt = 0;
    
    /* Process the variable args options. */
    {
        int a = 1;
        LIST * arg = lol_get( frame->args, a );
        while ( arg )
        {
            if ( strcmp("exit-status", arg->string) == 0 )
            {
                exit_status_opt = 1;
            }
            else if ( strcmp("no-output", arg->string) == 0 )
            {
                no_output_opt = 1;
            }
            arg = lol_get( frame->args, ++a );
        }
    }

    string_new( &s );

    fflush(NULL);

    p = popen(command->string, "r");
    if ( p == NULL )
        return L0;

    while ( (ret = fread(buffer, sizeof(char), sizeof(buffer)-1, p)) > 0 )
    {
        buffer[ret] = 0;
        if ( ! no_output_opt )
        {
            string_append( &s, buffer );
        }
    }

    exit_status = pclose(p);

    /* The command output is returned first. */
    result = list_new( L0, newstr(s.value) );
    string_free(&s);
    
    /* The command exit result next. */
    if ( exit_status_opt )
    {
        sprintf (buffer, "%d", exit_status);
        result = list_new( result, newstr( buffer ) );
    }
    
    return result;
}

#else

LIST *builtin_shell( PARSE *parse, FRAME *frame )
{
    return L0;
}

#endif
