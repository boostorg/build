/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "lists.h"
# include "variable.h"
# include "expand.h"
# include "filesys.h"
# include "newstr.h"
# include "strings.h"

# include <assert.h>
# include <string.h>
/*
 * expand.c - expand a buffer, given variable values
 *
 * External routines:
 *
 *     var_expand() - variable-expand input string into list of strings
 *
 * Internal routines:
 *
 *     var_edit() - copy input target name to output, performing : modifiers
 *     var_mods() - parse : modifiers into FILENAME structure
 *
 * 01/25/94 (seiwald) - $(X)$(UNDEF) was expanding like plain $(X)
 * 04/13/94 (seiwald) - added shorthand L0 for null list pointer
 */

typedef struct {
	char	downshift;	/* :L -- downshift result */
	char	upshift;	/* :U -- upshift result */
	char	parent;		/* :P -- go to parent directory */
        char    to_slashes;     /* :T -- convert "\" to "/" */
} VAR_ACTS ;

static void var_edit( char *in, char *mods, string *out );
static void var_mods( char *mods, FILENAME *f, VAR_ACTS *acts );

static int adjust_index( int index, int length );

# define MAGIC_COLON	'\001'
# define MAGIC_LEFT	'\002'
# define MAGIC_RIGHT	'\003'

/*
 * var_expand() - variable-expand input string into list of strings
 *
 * Would just copy input to output, performing variable expansion, 
 * except that since variables can contain multiple values the result
 * of variable expansion may contain multiple values (a list).  Properly
 * performs "product" operations that occur in "$(var1)xxx$(var2)" or
 * even "$($(var2))".
 *
 * Returns a newly created list.
 */

LIST *
var_expand( 
	LIST	*l,
	char	*in,
	char	*end,
	LOL	*lol,
	int	cancopyin )
{
        string buf[1];
        size_t prefix_length;
	char *out;
	char *inp = in;
	char *ov;		/* for temp copy of variable in outbuf */
	int depth;

	if( DEBUG_VAREXP )
	    printf( "expand '%.*s'\n", end - in, in );

	/* This gets alot of cases: $(<) and $(>) */

	if( in[0] == '$' && in[1] == '(' && in[3] == ')' && !in[4] )
	{
	    switch( in[2] )
	    {
	    case '1':
	    case '<':
		return list_copy( l, lol_get( lol, 0 ) );

	    case '2':
	    case '>':
		return list_copy( l, lol_get( lol, 1 ) );
	    }
	}

	/* See if we can use a simple copy of in to out. */

	while ( in < end )
	    if ( *in++ == '$' && *in == '(' )
		goto expand;

	/* No variables expanded - just add copy of input string to list. */

	/* Cancopyin is an optimization: if the input was already a list */
	/* item, we can use the copystr() to put it on the new list. */
	/* Otherwise, we use the slower newstr(). */

	if ( cancopyin )
        {
	    return list_new( l, copystr( inp ) );
        }
	else
        {
            LIST* r;
            string_new( buf );
            string_append_range( buf, inp, in );
            
	    r = list_new( l, newstr( buf->value ) );
            string_free( buf );
            return r;
        }

    expand:
        string_new( buf );
        string_append_range( buf, inp, in - 1 ); /* copy in initial stuff */
	/*
         * Input so far (ignore blanks):
         *
         *      stuff-in-outbuf $(variable) remainder
         *                       ^                   ^
         *                       in                  end
         * Output so far:
         *
         *      stuff-in-outbuf $
         *      ^                ^
         *      out_buf          out
         *
         *
         * We just copied the $ of $(...), so back up one on the output.
         * We now find the matching close paren, copying the variable and
         * modifiers between the $( and ) temporarily into out_buf, so that
         * we can replace :'s with MAGIC_COLON.  This is necessary to avoid
         * being confused by modifier values that are variables containing
         * :'s.  Ugly.
         */

	depth = 1;
	inp = ++in; /* skip over the '(' */

	while( in < end && depth )
	{
	    switch( *in++ )
	    {
	    case '(': depth++; break;
	    case ')': depth--; break;
	    }
	}

	/*
	 * Input so far (ignore blanks):
	 *
	 *	stuff-in-outbuf $(variable) remainder
	 *			  ^	   ^         ^
	 *			  inp      in        end
         */
        prefix_length = buf->size;
        string_append_range( buf, inp, in - 1 );

        out = buf->value + prefix_length;
	for ( ov = out; ov < buf->value + buf->size; ++ov )
	{
	    switch( *ov )
	    {
	    case ':': *ov = MAGIC_COLON; break;
	    case '[': *ov = MAGIC_LEFT; break;
	    case ']': *ov = MAGIC_RIGHT; break;
	    }
	}

	/*
	 * Input so far (ignore blanks):
	 *
	 *	stuff-in-outbuf $(variable) remainder
	 *			            ^        ^
	 *			            in       end
	 * Output so far:
	 *
	 *	stuff-in-outbuf variable
	 *	^	        ^       ^
	 *	out_buf         out	ov
	 *
	 * Later we will overwrite 'variable' in out_buf, but we'll be
	 * done with it by then.  'variable' may be a multi-element list, 
	 * so may each value for '$(variable element)', and so may 'remainder'.
	 * Thus we produce a product of three lists.
	 */

	{
	    LIST *variables = 0;
	    LIST *remainder = 0;
	    LIST *vars;

	    /* Recursively expand variable name & rest of input */

	    if( out < ov )
		variables = var_expand( L0, out, ov, lol, 0 );
	    if( in < end )
		remainder = var_expand( L0, in, end, lol, 0 );

	    /* Now produce the result chain */

	    /* For each variable name */

	    for( vars = variables; vars; vars = list_next( vars ) )
	    {
		LIST *value;
		char *colon;
		char *bracket;
		int i, sub1, sub2;
		string variable;
                char *varname;

		/* Look for a : modifier in the variable name */
		/* Must copy into varname so we can modify it */
                
                string_copy( &variable, vars->string );
                varname = variable.value;

		if( colon = strchr( varname, MAGIC_COLON ) )
                {
		    string_truncate( &variable, colon - varname );
                }

		if( bracket = strchr( varname, MAGIC_LEFT ) )
		{
		    char *dash = 0;

		    if( bracket[1] && ( dash = strchr( bracket + 2, '-' ) ) )
		    {
			string_truncate( &variable, dash - varname );
			sub1 = atoi( bracket + 1 );
			sub2 = atoi( dash + 1 );
		    }
		    else
		    {
			sub1 = sub2 = atoi( bracket + 1 );
		    }

                    string_truncate( &variable, bracket - varname );
		}
		else
		{
		    sub1 = sub2 = 0;	/* not needed */
		}

		/* Get variable value, specially handling $(<), $(>), $(n) */
		
		if( varname[0] == '<' && !varname[1] )
		{
		    value = lol_get( lol, 0 );
		}
		else if( varname[0] == '>' && !varname[1] )
		{
		    value = lol_get( lol, 1 );
		}
		else if( varname[0] >= '1' && varname[0] <= '9' && !varname[1] )
		{
		    value = lol_get( lol, varname[0] - '1' );
		}
		else 
		{
		    value = var_get( varname );
		}
                
		/* The fast path: $(x) - just copy the variable value. */

		if( out == buf->value && !bracket && !colon && in == end )
		{
                    string_free( &variable );
		    l = list_copy( l, value );
		    continue;
		}

                /* Adjust negative indices */
                if ( sub1 < 0 || sub2 < 0 )
                {
                    int length = list_length( value );
                    sub1 = adjust_index( sub1, length );
                    sub2 = adjust_index( sub2, length );
                }

		/* For each variable value */
		for( i = 1; value; i++, value = list_next( value ) )
		{
		    LIST *rem;
                    size_t postfix_start;

		    /* Skip members not in subscript */

		    if( bracket && ( i < sub1 || sub2 && i > sub2 ) )
			continue;

                    string_truncate( buf, prefix_length );

		    /* Apply : mods, if present */

		    if( colon )
			var_edit( value->string, colon + 1, buf );
		    else
                        string_append( buf, value->string );

		    /* If no remainder, append result to output chain. */

		    if( in == end )
		    {
			l = list_new( l, newstr( buf->value ) );
			continue;
		    }

		    /* Remember the end of the variable expansion so */
		    /* we can just tack on each instance of 'remainder' */

		    postfix_start = buf->size;

		    /* For each remainder, or just once if no remainder, */
		    /* append the complete string to the output chain */

		    for( rem = remainder; rem; rem = list_next( rem ) )
		    {
                        string_truncate( buf, postfix_start );
                        string_append( buf, rem->string );
			l = list_new( l, newstr( buf->value ) );
		    }
		}
                string_free( &variable );
	    }

	    /* variables & remainder were gifts from var_expand */
	    /* and must be freed */

	    if( variables )
		list_free( variables );
	    if( remainder)
		list_free( remainder );

	    if( DEBUG_VAREXP )
	    {
		printf( "expanded to " );
		list_print( l );
		printf( "\n" );
	    }

            string_free( buf );
	    return l;
	}
}

/*
 * var_edit() - copy input target name to output, performing : modifiers
 */
	
static void
var_edit( 
	char	*in,
	char	*mods,
	string	*out)
{
	FILENAME oldf, newf;
	VAR_ACTS acts;

	/* Parse apart original filename, putting parts into "oldf" */

	file_parse( in, &oldf );

	/* Parse apart modifiers, putting them into "newf" */

	var_mods( mods, &newf, &acts );

	/* Replace any oldf with newf */

	if( newf.f_grist.ptr )
	    oldf.f_grist = newf.f_grist;

	if( newf.f_root.ptr )
	    oldf.f_root = newf.f_root;

	if( newf.f_dir.ptr )
	    oldf.f_dir = newf.f_dir;

	if( newf.f_base.ptr )
	    oldf.f_base = newf.f_base;

	if( newf.f_suffix.ptr )
	    oldf.f_suffix = newf.f_suffix;

	if( newf.f_member.ptr )
	    oldf.f_member = newf.f_member;

	/* If requested, modify oldf to point to parent */

	if( acts.parent )
	    file_parent( &oldf );

	/* Put filename back together */

	file_build( &oldf, out, 0 );

	/* Handle upshifting, downshifting now */
        /* Handle conversion of "\" to "/" */
        {
            char* p;
            for ( p = out->value; *p; ++p)
            {
                if( acts.upshift )
                {
                    *p = toupper( *p );
                }
                else if( acts.downshift )
                {
                    *p = tolower( *p );
                }
                if ( acts.to_slashes )
                {
                    if ( *p == '\\' )
                        *p = '/';
                }
            }
            out->size = p - out->value;
        }
}


/*
 * var_mods() - parse : modifiers into FILENAME structure
 *
 * The : modifiers in a $(varname:modifier) currently support replacing
 * or omitting elements of a filename, and so they are parsed into a 
 * FILENAME structure (which contains pointers into the original string).
 *
 * Modifiers of the form "X=value" replace the component X with
 * the given value.  Modifiers without the "=value" cause everything 
 * but the component X to be omitted.  X is one of:
 *
 *	G <grist>
 *	D directory name
 *	B base name
 *	S .suffix
 *	M (member)
 *	R root directory - prepended to whole path
 *
 * This routine sets:
 *
 *	f->f_xxx.ptr = 0
 *	f->f_xxx.len = 0
 *		-> leave the original component xxx
 *
 *	f->f_xxx.ptr = string
 *	f->f_xxx.len = strlen( string )
 *		-> replace component xxx with string
 *
 *	f->f_xxx.ptr = ""
 *	f->f_xxx.len = 0
 *		-> omit component xxx
 *
 * var_edit() above and file_build() obligingly follow this convention.
 */

static void
var_mods(
	char		*mods,
	FILENAME	*f,
	VAR_ACTS	*acts )
{
	char *flags = "GRDBSMT";
	int havezeroed = 0;
	memset( (char *)f, 0, sizeof( *f ) );
	memset( (char *)acts, 0, sizeof( *acts ) );

	while( *mods )
	{
	    char *fl;
	    FILEPART *fp;

	    /* First take care of :U or :L (upshift, downshift) */

	    if( *mods == 'L' )
	    {
		acts->downshift = 1;
		++mods;
		continue;
	    }
	    else if( *mods == 'U' )
	    {
		acts->upshift = 1;
		++mods;
		continue;
	    }
	    else if( *mods == 'P' )
	    {
		acts->parent = 1;
		++mods;
		continue;
	    }
            else if ( *mods == 'T' )
            {
              acts->to_slashes = 1;
              ++mods;
              continue;
            }

	    /* Now handle the file component flags */

	    if( !( fl = strchr( flags, *mods++ ) ) )
		break;	/* should complain, but so what... */

	    fp = &f->part[ fl - flags ];

	    if( *mods++ != '=' )
	    {
		/* :X - turn everything but X off */

		int i;

		mods--;

		if( !havezeroed++ )
		    for( i = 0; i < 6; i++ )
		{
		    f->part[ i ].len = 0;
		    f->part[ i ].ptr = "";
		}

		fp->ptr = 0;
	    }
	    else
	    {
		/* :X=value - set X to value */

		char *p;

		if( p = strchr( mods, MAGIC_COLON ) )
		{
		    fp->ptr = mods;
		    fp->len = p - mods;
		    mods = p + 1;
		}
		else
		{
		    fp->ptr = mods;
		    fp->len = strlen( mods );
		    mods += fp->len;
		}
	    }
	}
}

static int adjust_index( int index, int length )
{
    if ( index < 0 )
        index = length + 1 + index;
    if ( index < 0 )
        index = 0;
    return index;
}

#ifndef NDEBUG
void var_expand_unit_test()
{
    LOL lol[1];
    LIST* l, *l2;
    LIST *expected = list_new( list_new( L0, newstr( "axb" ) ), newstr( "ayb" ) );
    LIST *e2;
    char axyb[] = "a$(xy)b";
    char azb[] = "a$($(z))b";
    
    lol_init(lol);
    var_set("xy", list_new( list_new( L0, newstr( "x" ) ), newstr( "y" ) ), VAR_SET );
    var_set("z", list_new( L0, newstr( "xy" ) ), VAR_SET );
    
    l = var_expand( 0, axyb, axyb + sizeof(axyb) - 1, lol, 0 );
    for ( l2 = l, e2 = expected; l2 && e2; l2 = list_next(l2), e2 = list_next(e2) )
        assert( !strcmp( e2->string, l2->string ) );
    list_free(l);
    
    l = var_expand( 0, azb, azb + sizeof(azb) - 1, lol, 0 );
    for ( l2 = l, e2 = expected; l2 && e2; l2 = list_next(l2), e2 = list_next(e2) )
        assert( !strcmp( e2->string, l2->string ) );
    list_free(l);
    
    lol_free(lol);
}
#endif
