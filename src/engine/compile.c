/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  (C) Copyright David Abrahams 2001. Permission to copy, use,
 *  modify, sell and distribute this software is granted provided this
 *  copyright notice appears in all copies. This software is provided
 *  "as is" without express or implied warranty, and with no claim as
 *  to its suitability for any purpose.
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "compile.h"
# include "variable.h"
# include "expand.h"
# include "rules.h"
# include "newstr.h"
# include "make.h"
# include "search.h"
# include "hdrmacro.h"
# include "hash.h"
# include "modules.h"
# include "strings.h"

# include <time.h>
# include <assert.h>

/*
 * compile.c - compile parsed jam statements
 *
 * External routines:
 *
 *	compile_append() - append list results of two statements
 *	compile_foreach() - compile the "for x in y" statement
 *	compile_if() - compile 'if' rule
 *	compile_while() - compile 'while' rule
 *	compile_include() - support for 'include' - call include() on file
 *	compile_list() - expand and return a list 
 *	compile_local() - declare (and set) local variables
 *	compile_null() - do nothing -- a stub for parsing
 *	compile_rule() - compile a single user defined rule
 *	compile_rules() - compile a chain of rules
 *	compile_set() - compile the "set variable" statement
 *	compile_setcomp() - support for `rule` - save parse tree 
 *	compile_setexec() - support for `actions` - save execution string 
 *	compile_settings() - compile the "on =" (set variable on exec) statement
 *	compile_switch() - compile 'switch' rule
 *
 * Internal routines:
 *
 *	debug_compile() - printf with indent to show rule expansion.
 *
 *	evaluate_if() - evaluate if to determine which leg to compile
 *	evaluate_rule() - execute a rule invocation
 *
 *	builtin_depends() - DEPENDS/INCLUDES rule
 *	builtin_echo() - ECHO rule
 *	builtin_exit() - EXIT rule
 *	builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *
 * 02/03/94 (seiwald) -	Changed trace output to read "setting" instead of 
 *			the awkward sounding "settings".
 * 04/12/94 (seiwald) - Combined build_depends() with build_includes().
 * 04/12/94 (seiwald) - actionlist() now just appends a single action.
 * 04/13/94 (seiwald) - added shorthand L0 for null list pointer
 * 05/13/94 (seiwald) - include files are now bound as targets, and thus
 *			can make use of $(SEARCH)
 * 06/01/94 (seiwald) - new 'actions existing' does existing sources
 * 08/23/94 (seiwald) - Support for '+=' (append to variable)
 * 12/20/94 (seiwald) - NOTIME renamed NOTFILE.
 * 01/22/95 (seiwald) - Exit rule.
 * 02/02/95 (seiwald) - Always rule; LEAVES rule.
 * 02/14/95 (seiwald) - NoUpdate rule.
 * 09/11/00 (seiwald) - new evaluate_rule() for headers().
 * 09/11/00 (seiwald) - compile_xxx() now return LIST *.
 *			New compile_append() and compile_list() in
 *			support of building lists here, rather than
 *			in jamgram.yy.
 */

static void debug_compile( int which, char *s );

static int evaluate_if( PARSE *parse, FRAME *frame );

static LIST *builtin_depends( PARSE *parse, FRAME *frame );
static LIST *builtin_echo( PARSE *parse, FRAME *frame );
static LIST *builtin_exit( PARSE *parse, FRAME *frame );
static LIST *builtin_flags( PARSE *parse, FRAME *frame );
static LIST *builtin_hdrmacro( PARSE *parse, FRAME *frame );
static LIST *builtin_import( PARSE *parse, FRAME *frame );
static LIST *builtin_caller_module( PARSE *parse, FRAME *frame );
LIST *builtin_subst( PARSE  *parse, FRAME *frame );

int glob( char *s, char *c );


void frame_init( FRAME* frame )
{
    frame->prev = 0;
    lol_init(frame->args);
    frame->module = root_module();
}

void frame_free( FRAME* frame )
{
    lol_free( frame->args );
}

/*
 * compile_builtin() - define builtin rules
 */

# define P0 (PARSE *)0
# define C0 (char *)0

static void lol_build( LOL* lol, char** elements )
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

static RULE* bind_builtin( char* name, LIST*(*f)(PARSE*, FRAME*), int flags, char** args )
{
    argument_list* arg_list = 0;
    RULE* builtin_rule;
    
    if ( args )
    {
        arg_list = args_new();
        lol_build( arg_list->data, args );
    }

    return new_rule_body( root_module(), name, arg_list,
                                  parse_make( f, P0, P0, P0, C0, C0, flags ) );
}

static RULE* duplicate_rule( char* name, RULE* other )
{
    return import_rule( other, root_module(), name );
}

void
compile_builtins()
{
    duplicate_rule( "Always", bind_builtin( "ALWAYS", builtin_flags, T_FLAG_TOUCHED, 0 ) );
    duplicate_rule( "Depends", bind_builtin( "DEPENDS", builtin_depends, T_DEPS_DEPENDS, 0 ) );
    duplicate_rule( "Echo", bind_builtin( "ECHO", builtin_echo, 0, 0 ) );
    duplicate_rule( "Exit", bind_builtin( "EXIT", builtin_exit, 0, 0 ) );
    duplicate_rule( "Includes", bind_builtin( "INCLUDES", builtin_depends, T_DEPS_INCLUDES, 0 ) );
    duplicate_rule( "HdrMacro", bind_builtin( "HDRMACRO", builtin_hdrmacro, 0, 0 ) );
    duplicate_rule( "Leaves", bind_builtin( "LEAVES", builtin_flags, T_FLAG_LEAVES, 0 ) );
    duplicate_rule( "NoCare", bind_builtin( "NOCARE", builtin_flags, T_FLAG_NOCARE, 0 ) );
    duplicate_rule( "NOTIME",
        duplicate_rule( "NotFile",
            bind_builtin( "NOTFILE", builtin_flags, T_FLAG_NOTFILE, 0 ) ) );

    duplicate_rule( "NoUpdate", bind_builtin( "NOUPDATE", builtin_flags, T_FLAG_NOUPDATE, 0 ) );
    duplicate_rule( "Temporary", bind_builtin( "TEMPORARY", builtin_flags, T_FLAG_TEMP, 0 ) );

  /* FAIL_EXPECTED is an experimental built-in that is used to indicate        */
  /* that the result of a target build action should be inverted (ok <=> fail) */
  /* this can be useful when performing test runs from Jamfiles..              */
  /*                                                                           */
  /* Beware that this rule might disappear or be renamed in the future..       */
  /* contact david.turner@freetype.org for more details..                      */
    bind_builtin( "FAIL_EXPECTED", builtin_flags, T_FLAG_FAIL_EXPECTED, 0 );

    {
        char* args[] = { "string", "pattern", "replacements", "+", 0 };
        duplicate_rule( "subst", bind_builtin( "SUBST", builtin_subst, 0, args ) );
    }
    
    {
        char* args[] = {
            "target_module", "?"
            , ":", "source_module", "?"
            , ":", "rule_names", "*"
            , ":", "target_names", "*", 0
        };
        
        bind_builtin( "IMPORT", builtin_import, 0, args );
    }

    {
        char* args[] = { 0 };
        bind_builtin( "CALLER_MODULE", builtin_caller_module, 0, args );
    }
}

/*
 * compile_append() - append list results of two statements
 *
 *	parse->left	more compile_append() by left-recursion
 *	parse->right	single rule
 */

LIST *
compile_append(
	PARSE	*parse,
	FRAME *frame )
{
	/* Append right to left. */

	return list_append( 
		(*parse->left->func)( parse->left, frame ),
		(*parse->right->func)( parse->right, frame ) );
}

/*
 * compile_foreach() - compile the "for x in y" statement
 *
 * Compile_foreach() resets the given variable name to each specified
 * value, executing the commands enclosed in braces for each iteration.
 *
 *	parse->string	index variable
 *	parse->left	variable values
 *	parse->right	rule to compile
 */

LIST *
compile_foreach(
	PARSE	*parse,
	FRAME *frame )
{
	LIST	*nv = (*parse->left->func)( parse->left, frame );
	LIST	*l;
	SETTINGS *s = 0;
        
        if ( parse->num )
        {
            s = addsettings( s, 0, parse->string, L0 );
            pushsettings( s );
        }

	/* Call var_set to reset $(parse->string) for each val. */

	for( l = nv; l; l = list_next( l ) )
	{
	    LIST *val = list_new( L0, copystr( l->string ) );

	    var_set( parse->string, val, VAR_SET );

	    list_free( (*parse->right->func)( parse->right, frame ) );
	}

        if ( parse->num )
            popsettings( s );

	list_free( nv );

	return L0;
}

/*
 * compile_if() - compile 'if' rule
 *
 *	parse->left		condition tree
 *	parse->right		then tree
 *	parse->third		else tree
 */

LIST *
compile_if(
	PARSE	*p,
	FRAME *frame )
{
	if( evaluate_if( p->left, frame ) )
	{
	    return (*p->right->func)( p->right, frame );
	}
	else
	{
	    return (*p->third->func)( p->third, frame );
	}
}

LIST *
compile_while(
	PARSE	*p,
	FRAME *frame )
{
    while ( evaluate_if( p->left, frame ) )
    {
        list_free( (*p->right->func)( p->right, frame ) );
    }
    return L0;
}

/*
 * evaluate_if() - evaluate if to determine which leg to compile
 *
 * Returns:
 *	!0	if expression true - compile 'then' clause
 *	0	if expression false - compile 'else' clause
 */

static int
evaluate_if(
	PARSE	*parse,
	FRAME *frame )
{
	int	status;

	if( parse->num <= COND_OR )
	{
	    /* Handle one of the logical operators */

	    switch( parse->num )
	    {
	    case COND_NOT:
		status = !evaluate_if( parse->left, frame );
		break;

	    case COND_AND:
		status = evaluate_if( parse->left, frame ) &&
			 evaluate_if( parse->right, frame );
		break;

	    case COND_OR:
		status = evaluate_if( parse->left, frame ) ||
			 evaluate_if( parse->right, frame );
		break;

	    default:
		status = 0;	/* can't happen */
	    }
	}
	else
	{
	    /* Handle one of the comparison operators */
	    /* Expand targets and sources */

	    LIST *nt = (*parse->left->func)( parse->left, frame );
	    LIST *ns = (*parse->right->func)( parse->right, frame );

	    /* "a in b" make sure each of a is equal to something in b. */
	    /* Otherwise, step through pairwise comparison. */

	    if( parse->num == COND_IN )
	    {
		LIST *s, *t;

		/* Try each t until failure. */

		for( status = 1, t = nt; status && t; t = list_next( t ) )
		{
		    int stat1;

		    /* Try each s until success */

		    for( stat1 = 0, s = ns; !stat1 && s; s = list_next( s ) )
			stat1 = !strcmp( t->string, s->string );

		    status = stat1;
		}
	    }
	    else
	    {
		LIST *s = ns, *t = nt;

		status = 0;

		while( !status && ( t || s ) )
		{
		    char *st = t ? t->string : "";
		    char *ss = s ? s->string : "";

		    status = strcmp( st, ss );

		    t = t ? list_next( t ) : t;
		    s = s ? list_next( s ) : s;
		}
	    }

	    switch( parse->num )
	    {
	    case COND_EXISTS:	status = status > 0 ; break;
	    case COND_EQUALS:	status = !status; break;
	    case COND_NOTEQ:	status = status != 0; break;
	    case COND_LESS:	status = status < 0; break;
	    case COND_LESSEQ:	status = status <= 0; break;
	    case COND_MORE:	status = status > 0; break;
	    case COND_MOREEQ:	status = status >= 0; break;
	    case COND_IN:	/* status = status */ break;
	    }

	    if( DEBUG_IF )
	    {
		debug_compile( 0, "if" );
		list_print( nt );
		printf( "(%d)", status );
		list_print( ns );
		printf( "\n" );
	    }

	    list_free( nt );
	    list_free( ns );

	}

	return status;
}

/*
 * compile_include() - support for 'include' - call include() on file
 *
 * 	parse->left	list of files to include (can only do 1)
 */

LIST *
compile_include(
	PARSE	*parse,
	FRAME *frame )
{
	LIST	*nt = (*parse->left->func)( parse->left, frame );

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "include" );
	    list_print( nt );
	    printf( "\n" );
	}

	if( nt )
	{
	    TARGET *t = bindtarget( nt->string );

            /* DWA 2001/10/22 - Perforce Jam clears the arguments here, which
             * prevents an included file from being treated as part of the body
             * of a rule. I didn't see any reason to do that, so I lifted the
             * restriction.
             */
               
	    /* Bind the include file under the influence of */
	    /* "on-target" variables.  Though they are targets, */
	    /* include files are not built with make(). */

	    pushsettings( t->settings );
	    t->boundname = search( t->name, &t->time );
	    popsettings( t->settings );

	    parse_file( t->boundname, frame );
	}

	list_free( nt );

	return L0;
}

LIST *
compile_module(
	PARSE	*p,
	FRAME *frame )
{
    /* Here we are entering a module declaration block. 
     */
    LIST* module_name = (*p->left->func)( p->left, frame );
    LIST* result;

    module* outer_module = frame->module;
    frame->module = module_name ? bindmodule( module_name->string ) : root_module();

    if ( outer_module != frame->module )
    {
        exit_module( outer_module );
        enter_module( frame->module );
    }
    
    result = (*p->right->func)( p->right, frame );
    
    if ( outer_module != frame->module )
    {
        exit_module( frame->module );
        enter_module( outer_module );
        frame->module = outer_module;
    }
    
    list_free( module_name );
    return result;
}


/*
 * compile_list() - expand and return a list 
 *
 * 	parse->string - character string to expand
 */

LIST *
compile_list(
	PARSE	*parse,
	FRAME *frame )
{
	/* voodoo 1 means: s is a copyable string */
	char *s = parse->string;
	return var_expand( L0, s, s + strlen( s ), frame->args, 1 );
}

/*
 * compile_local() - declare (and set) local variables
 *
 *	parse->left	list of variables
 *	parse->right	list of values
 *	parse->third	rules to execute
 */

LIST *
compile_local(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *l;
	SETTINGS *s = 0;
	LIST	*nt = (*parse->left->func)( parse->left, frame );
	LIST	*ns = (*parse->right->func)( parse->right, frame );
	LIST	*result;

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "local" );
	    list_print( nt );
	    printf( " = " );
	    list_print( ns );
	    printf( "\n" );
	}

	/* Initial value is ns */

	for( l = nt; l; l = list_next( l ) )
	    s = addsettings( s, 0, l->string, list_copy( (LIST*)0, ns ) );

	list_free( ns );
	list_free( nt );

	/* Note that callees of the current context get this "local" */
	/* variable, making it not so much local as layered. */

	pushsettings( s );
	result = (*parse->third->func)( parse->third, frame );
	popsettings( s );

	freesettings( s );

	return result;
}

/*
 * compile_null() - do nothing -- a stub for parsing
 */

LIST *
compile_null(
	PARSE	*parse,
	FRAME *frame )
{
	return L0;
}

/*
 * compile_rule() - compile a single user defined rule
 *
 *	parse->string	name of user defined rule
 *	parse->left	parameters (list of lists) to rule, recursing left
 *
 * Wrapped around evaluate_rule() so that headers() can share it.
 */

LIST *
compile_rule(
    PARSE	*parse,
    FRAME *frame )
{
    FRAME       inner[1];
    LIST	*result;
    PARSE	*p;
    

    /* Build up the list of arg lists */

    frame_init( inner );
    inner->prev = frame;
    inner->module = frame->module; /* This gets fixed up in evaluate_rule(), below */

    for( p = parse->left; p; p = p->left )
        lol_add( inner->args, (*p->right->func)( p->right, frame ) );

    /* And invoke rule */

    result = evaluate_rule( parse->string, inner );

    frame_free( inner );

    return result;
}

static void argument_error( char* message, RULE* rule, LOL* actual, LIST* arg )
{
    printf( "### argument error\n# rule %s ( ", rule->name );
    lol_print( rule->arguments->data );
    printf( ")\n# called with: ( " );
    lol_print( actual );
    printf( ")\n# %s %s", message, arg ? arg->string : "" );
    printf("\n");
    exit(1);
}


/*
 * collect_arguments() - local argument checking and collection
 */
static SETTINGS *
collect_arguments( RULE* rule, LOL* all_actual )
{
    SETTINGS *locals = 0;
    
    LOL *all_formal = rule->arguments ? rule->arguments->data : 0;
    if ( all_formal ) /* Nothing to set; nothing to check */
    {
        int max = all_formal->count > all_actual->count
            ? all_formal->count
            : all_actual->count;
        
        int n;
        for ( n = 0; n < max ; ++n )
        {
            LIST *formal = lol_get( all_formal, n );
            LIST *actual = lol_get( all_actual, n );
            while ( formal )
            {
                char* name = formal->string;
                char modifier = 0;
                LIST* value = 0;
                if ( formal->next )
                {
                    char *next = formal->next->string;
                    if ( next && next[0] != 0 && next[1] == 0 )
                        modifier = next[0];
                }
                
                if ( !actual && modifier != '?' && modifier != '*' )
                {
                    argument_error( "missing argument", rule, all_actual, formal );
                }

                switch ( modifier )
                {
                case '+':
                case '*':
                    value = list_copy( 0, actual );
                    actual = 0;
                    /* skip an extra element for the modifier */
                    formal = formal->next; 
                    break;
                case '?':
                    /* skip an extra element for the modifier */
                    formal = formal->next; 
                    /* fall through */
                default:
                    if ( actual ) /* in case actual is missing */
                    {
                        value = list_new( 0, actual->string );
                        actual = actual->next;
                    }
                }
                
                locals = addsettings( locals, 0, name, value );
                formal = formal->next;
            }
            
            if ( actual )
            {
                argument_error( "extra argument", rule, all_actual, actual );
            }
        }
    }
    return locals;
}

struct profile_info
{
    char* name;                 /* name of rule being called */
    clock_t cumulative;         /* cumulative time spent in rule */
    clock_t subrules;           /* time spent in subrules */
    unsigned long num_entries;  /* number of time rule was entered */
};
typedef struct profile_info profile_info;

struct profile_frame
{
    profile_info* info;               /* permanent storage where data accumulates */
    clock_t overhead;                 /* overhead for profiling in this call */
    clock_t entry_time;               /* time of last entry to rule */
    struct profile_frame* caller;     /* stack frame of caller */
};
typedef struct profile_frame profile_frame;

static profile_frame* profile_stack = 0;
static struct hash* profile_hash = 0;

static void profile_enter( char* rulename, profile_frame* frame )
{
    clock_t start = clock();
    profile_info info, *p = &info;
    
    if ( !profile_hash )
        profile_hash = hashinit(sizeof(profile_info), "profile");

    info.name = rulename;
    
    if ( hashenter( profile_hash, (HASHDATA **)&p ) )
        p->cumulative = p->subrules = p->num_entries = 0;
    
    ++(p->num_entries);
    
    frame->info = p;
    
    frame->caller = profile_stack;
    profile_stack = frame;

    frame->entry_time = clock();
    frame->overhead = 0;

    /* caller pays for the time it takes to play with the hash table */
    if ( frame->caller )
        frame->caller->overhead += frame->entry_time - start;
}
    
static void profile_exit(profile_frame* frame)
{
    /* cumulative time for this call */
    clock_t t = clock() - frame->entry_time - frame->overhead;
    frame->info->cumulative += t;
    
    if (frame->caller)
    {
        /* caller's cumulative time must account for this overhead */
        frame->caller->overhead += frame->overhead;
        frame->caller->info->subrules += t;
    }
    /* pop this stack frame */
    profile_stack = frame->caller;
}

static void dump_profile_entry(void* p_, void* ignored)
{
    profile_info* p = p_;
    clock_t total = p->cumulative;
    printf("%10d %10d %10d %s\n", total, total - p->subrules, p->num_entries, p->name);
}

void profile_dump()
{
    if ( profile_hash )
    {
        printf("%10s %10s %10s %s\n", "gross", "net", "# entries", "name");
        hashenumerate( profile_hash, dump_profile_entry, 0 );
    }
}

/*
 * evaluate_rule() - execute a rule invocation
 */

LIST *
evaluate_rule(
	char	*rulename,
	FRAME *frame )
{
    LIST	  *result = L0;
    RULE          *rule;
    profile_frame prof[1];
    module    *prev_module = frame->module;
    
    LIST*  l = var_expand( L0, rulename, rulename+strlen(rulename), frame->args, 0 );

    if ( !l )
    {
        printf( "warning: rulename %s expands to empty string\n", rulename );
        return result;
    }

    if ( DEBUG_COMPILE )
    {
        debug_compile( 1, l->string );
        lol_print( frame->args );
        printf( "\n" );
    }
    rulename = l->string;
    rule = bindrule( l->string, frame->module );

    if ( rule->procedure && rule->procedure->module != prev_module )
    {
        /* propagate current module to nested rule invocations */
        frame->module = rule->procedure->module;
        
        /* swap variables */
        exit_module( prev_module );
        enter_module( rule->procedure->module );
    }

    list_free( l );
    
    if ( DEBUG_PROFILE && rule->procedure )
        profile_enter( rule->procedure->rulename, prof );

    /* Check traditional targets $(<) and sources $(>) */

    if( !rule->actions && !rule->procedure )
        printf( "warning: unknown rule %s\n", rule->name );

    /* If this rule will be executed for updating the targets */
    /* then construct the action for make(). */

    if( rule->actions )
    {
        TARGETS	*t;
        ACTION	*action;

        /* The action is associated with this instance of this rule */

        action = (ACTION *)malloc( sizeof( ACTION ) );
        memset( (char *)action, '\0', sizeof( *action ) );

        action->rule = rule;
        action->targets = targetlist( (TARGETS *)0, lol_get( frame->args, 0 ) );
        action->sources = targetlist( (TARGETS *)0, lol_get( frame->args, 1 ) );

        /* Append this action to the actions of each target */

        for( t = action->targets; t; t = t->next )
            t->target->actions = actionlist( t->target->actions, action );
    }

    /* Now recursively compile any parse tree associated with this rule */
    /* refer/free to ensure rule not freed during use */

    if( rule->procedure )
    {
        SETTINGS *local_args = collect_arguments( rule, frame->args );
        PARSE *parse = rule->procedure;
        parse_refer( parse );
        
        pushsettings( local_args );
        result = (*parse->func)( parse, frame );
        popsettings( local_args );
        
        parse_free( parse );
    }

    if ( frame->module != prev_module )
    {
        exit_module( frame->module );
        enter_module( prev_module );
    }

    if ( DEBUG_PROFILE && rule->procedure )
        profile_exit( prof );

    if( DEBUG_COMPILE )
        debug_compile( -1, 0 );

    return result;
}

/*
 * compile_rules() - compile a chain of rules
 *
 *	parse->left	more compile_rules() by left-recursion
 *	parse->right	single rule
 */

LIST *
compile_rules(
	PARSE	*parse,
	FRAME *frame )
{
	/* Ignore result from first statement; return the 2nd. */

	list_free( (*parse->left->func)( parse->left, frame ) );
	return (*parse->right->func)( parse->right, frame );
}

/*
 * compile_set() - compile the "set variable" statement
 *
 *	parse->left	variable names
 *	parse->right	variable values 
 *	parse->num	ASSIGN_SET/APPEND/DEFAULT
 */

LIST *
compile_set(
	PARSE	*parse,
	FRAME *frame )
{
	LIST	*nt = (*parse->left->func)( parse->left, frame );
	LIST	*ns = (*parse->right->func)( parse->right, frame );
	LIST	*l;
	int	setflag;
	char	*trace;

	switch( parse->num )
	{
	case ASSIGN_SET:	setflag = VAR_SET; trace = "="; break;
	case ASSIGN_APPEND:	setflag = VAR_APPEND; trace = "+="; break;
	case ASSIGN_DEFAULT:	setflag = VAR_DEFAULT; trace = "?="; break;
	default:		setflag = VAR_SET; trace = ""; break;
	}

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "set" );
	    list_print( nt );
	    printf( " %s ", trace );
	    list_print( ns );
	    printf( "\n" );
	}

	/* Call var_set to set variable */
	/* var_set keeps ns, so need to copy it */

	for( l = nt; l; l = list_next( l ) )
	    var_set( l->string, list_copy( L0, ns ), setflag );

	list_free( nt );

	return ns;
}

/*
 * compile_set_module() - compile the "module local set variable" statement
 *
 *	parse->left     variable names
 *	parse->right	variable values 
 */
LIST *
compile_set_module(
	PARSE	*parse,
	FRAME *frame )
{
	LIST	*nt = (*parse->left->func)( parse->left, frame );
	LIST	*ns = (*parse->right->func)( parse->right, frame );
	LIST	*l;

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "set module" );
        printf( "(%s)", frame->module->name );
	    list_print( nt );
	    printf( " = " );
	    list_print( ns );
	    printf( "\n" );
	}

	/* Call var_set to set variable */
	/* var_set keeps ns, so need to copy it */

	for( l = nt; l; l = list_next( l ) )
    {
        bind_module_var( frame->module, l->string );
	    var_set( l->string, list_copy( L0, ns ), VAR_SET );
    }

	list_free( nt );

	return ns;
}


/*
 * compile_setcomp() - support for `rule` - save parse tree 
 *
 *	parse->string	rule name
 *	parse->left	rules for rule
 *  parse->right optional list-of-lists describing arguments
 */

LIST *
compile_setcomp(
	PARSE	*parse,
	FRAME *frame)
{
    argument_list* arg_list = 0;
    
    /* Create new LOL describing argument requirements if supplied */
    if ( parse->right )
    {
        PARSE *p;
        arg_list = args_new();
        for( p = parse->right; p; p = p->left )
            lol_add( arg_list->data, (*p->right->func)( p->right, frame ) );
    }
    
    new_rule_body( frame->module, parse->string, arg_list, parse->left );
	return L0;
}

/*
 * compile_setexec() - support for `actions` - save execution string 
 *
 *	parse->string	rule name
 *	parse->string1	OS command string
 *	parse->num	flags
 *	parse->left	`bind` variables
 *
 * Note that the parse flags (as defined in compile.h) are transfered
 * directly to the rule flags (as defined in rules.h).
 */

LIST *
compile_setexec(
	PARSE	*parse,
	FRAME *frame )
{
	LIST* bindlist = (*parse->left->func)( parse->left, frame );

    new_rule_actions( frame->module, parse->string, parse->string1, bindlist, parse->num );

	return L0;
}

/*
 * compile_settings() - compile the "on =" (set variable on exec) statement
 *
 *	parse->left	variable names
 *	parse->right	target name 
 *	parse->third	variable value 
 *	parse->num	ASSIGN_SET/APPEND	
 */

LIST *
compile_settings(
	PARSE	*parse,
	FRAME *frame )
{
	LIST	*nt = (*parse->left->func)( parse->left, frame );
	LIST	*ns = (*parse->third->func)( parse->third, frame );
	LIST	*targets = (*parse->right->func)( parse->right, frame );
	LIST	*ts;
	int	append = parse->num == ASSIGN_APPEND;

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "set" );
	    list_print( nt );
	    printf( "on " );
	    list_print( targets );
	    printf( " %s ", append ? "+=" : "=" );
	    list_print( ns );
	    printf( "\n" );
	}

	/* Call addsettings to save variable setting */
	/* addsettings keeps ns, so need to copy it */
	/* Pass append flag to addsettings() */

	for( ts = targets; ts; ts = list_next( ts ) )
	{
	    TARGET 	*t = bindtarget( ts->string );
	    LIST	*l;

	    for( l = nt; l; l = list_next( l ) )
		t->settings = addsettings( t->settings, append, 
				l->string, list_copy( (LIST*)0, ns ) );
	}

	list_free( nt );
	list_free( targets );

	return ns;
}

/*
 * compile_switch() - compile 'switch' rule
 *
 *	parse->left	switch value (only 1st used)
 *	parse->right	cases
 *
 *	cases->left	1st case
 *	cases->right	next cases
 *
 *	case->string	argument to match
 *	case->left	parse tree to execute
 */

LIST *
compile_switch(
	PARSE	*parse,
	FRAME *frame )
{
	LIST	*nt = (*parse->left->func)( parse->left, frame );
	LIST	*result = 0;

	if( DEBUG_COMPILE )
	{
	    debug_compile( 0, "switch" );
	    list_print( nt );
	    printf( "\n" );
	}

	/* Step through cases */

	for( parse = parse->right; parse; parse = parse->right )
	{
	    if( !glob( parse->left->string, nt ? nt->string : "" ) )
	    {
		/* Get & exec parse tree for this case */
		parse = parse->left->left;
		result = (*parse->func)( parse, frame );
		break;
	    }
	}

	list_free( nt );

	return result;
}



/*
 * builtin_depends() - DEPENDS/INCLUDES rule
 *
 * The DEPENDS builtin rule appends each of the listed sources on the 
 * dependency list of each of the listed targets.  It binds both the 
 * targets and sources as TARGETs.
 */

static LIST *
builtin_depends(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *targets = lol_get( frame->args, 0 );
	LIST *sources = lol_get( frame->args, 1 );
	int which = parse->num;
	LIST *l;

	for( l = targets; l; l = list_next( l ) )
	{
	    TARGET *t = bindtarget( l->string );
	    t->deps[ which ] = targetlist( t->deps[ which ], sources );
	}

	return L0;
}

/*
 * builtin_echo() - ECHO rule
 *
 * The ECHO builtin rule echoes the targets to the user.  No other 
 * actions are taken.
 */

static LIST *
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

static LIST *
builtin_exit(
	PARSE	*parse,
	FRAME *frame )
{
	list_print( lol_get( frame->args, 0 ) );
	printf( "\n" );
	exit( EXITBAD ); /* yeech */
	return L0;
}

/*
 * builtin_flags() - NOCARE, NOTFILE, TEMPORARY rule
 *
 * Builtin_flags() marks the target with the appropriate flag, for use
 * by make0().  It binds each target as a TARGET.
 */

static LIST *
builtin_flags(
	PARSE	*parse,
	FRAME *frame )
{
	LIST *l = lol_get( frame->args, 0 );

	for( ; l; l = list_next( l ) )
	    bindtarget( l->string )->flags |= parse->num;

	return L0;
}


static LIST *
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


/*
 * builtin_import() - IMPORT ( TARGET_MODULE ? : SOURCE_MODULE ? : RULE_NAMES * : TARGET_NAMES * )
 *
 * The IMPORT rule imports rules from the SOURCE_MODULE into the
 * TARGET_MODULE. If either SOURCE_MODULE or TARGET_MODULE is not supplied, it
 * refers to the root module. If any RULE_NAMES are supplied, they specify which
 * rules from the SOURCE_MODULE to import, otherwise all rules are imported. The
 * rules are given the names in TARGET_NAMES; if not enough TARGET_NAMES are
 * supplied, the excess rules are given the names in RULE_NAMES. If RULE_NAMES
 * is not supplied, TARGET_NAMES is ignored.
 */

struct import_data
{
    module* target_module;
    LIST* target_names;
};
typedef struct import_data import_data;

static void import_rule1( void* r_, void* data_ )
{
    RULE* r = r_;
    import_data* data = data_;
    
    char* target_name = data->target_names ? data->target_names->string : r->name;
    if (data->target_names)
        data->target_names = list_next(data->target_names);
    
    import_rule( r, data->target_module, target_name );
}

static LIST *
builtin_import(
	PARSE	*parse,
	FRAME *frame )
{
    LIST *target_module_name = lol_get( frame->args, 0 );
    LIST *source_module_name = lol_get( frame->args, 1 );
    LIST *rule_names = lol_get( frame->args, 2 );
    LIST *target_names = lol_get( frame->args, 3 );

    module* target_module = bindmodule( target_module_name ? target_module_name->string : 0 );
    module* source_module = bindmodule( source_module_name ? source_module_name->string : 0 );
    
    if ( rule_names == 0 )
    {
        import_data data;
        data.target_module = target_module;
        data.target_names = target_names;
        hashenumerate( source_module->rules, import_rule1, &data );
    }
    else
    {
        LIST *old_name, *target_name;
            
        for ( old_name = rule_names, target_name = target_names;
              old_name;
              old_name = list_next( old_name )
                  , target_name = list_next( target_name ) )
        {
            RULE r_, *r = &r_;
            r_.name = old_name->string;
                
            if ( !target_name )
                target_name = old_name;

            if ( hashcheck( source_module->rules, (HASHDATA**)&r ) )
            {
                import_rule( r, target_module, target_name->string );
            }
        }
    }

    return L0;
}

/*
 * builtin_caller_module() - CALLER_MODULE ( )
 *
 * Returns the name of the module of the rule which called the one calling this
 * one, or, if no such module exists, returns the empty list. Also returns the
 * empty list when the module in question is the global module. This rule is
 * needed for implementing module import behavior.
 */
static LIST *builtin_caller_module( PARSE *parse, FRAME *frame )
{
    char buffer[4096] = "";
    int len;
    
    int i;
    for (i = 0; i < 2 && frame->prev; ++i)
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
 * debug_compile() - printf with indent to show rule expansion.
 */

static void
debug_compile( int which, char *s )
{
	static int level = 0;
	static char indent[36] = ">>>>|>>>>|>>>>|>>>>|>>>>|>>>>|>>>>|";
	
    if ( which >= 0 )
    {
      int i;
      
      i = (level+1)*2;
      while ( i > 35 )
      {
        printf( indent );
        i -= 35;
      }
      printf( "%*.*s ", i, i, indent );
    }

	if( s )
	    printf( "%s ", s );

	level += which;
}
