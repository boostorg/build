/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "filesys.h"

# ifdef OS_MAC

# define DELIM ':'

/*
 * pathunix.c - manipulate file names on UNIX, NT, OS2
 *
 * External routines:
 *
 *	file_parse() - split a file name into dir/base/suffix/member
 *	file_build() - build a filename given dir/base/suffix/member
 *	file_parent() - make a FILENAME point to its parent dir
 *
 * File_parse() and file_build() just manipuate a string and a structure;
 * they do not make system calls.
 *
 * 04/08/94 (seiwald) - Coherent/386 support added.
 * 12/26/93 (seiwald) - handle dir/.suffix properly in file_build()
 * 12/19/94 (mikem) - solaris string table insanity support
 * 12/21/94 (wingerd) Use backslashes for pathnames - the NT way.
 * 02/14/95 (seiwald) - parse and build /xxx properly
 * 02/23/95 (wingerd) Compilers on NT can handle "/" in pathnames, so we
 *                    should expect hdr searches to come up with strings
 *                    like "thing/thing.h". So we need to test for "/" as
 *                    well as "\" when parsing pathnames.
 * 03/16/95 (seiwald) - fixed accursed typo on line 69.
 * 05/03/96 (seiwald) - split from filent.c, fileunix.c
 * 12/20/96 (seiwald) - when looking for the rightmost . in a file name,
 *		      don't include the archive member name.
 */

/*
 * file_parse() - split a file name into dir/base/suffix/member
 */

void
file_parse( 
	char	*file,
	FILENAME *f )
{
	char *p, *q;
	char *end;
	
	memset( (char *)f, 0, sizeof( *f ) );

	/* Look for <grist> */

	if( file[0] == '<' && ( p = strchr( file, '>' ) ) )
	{
	    f->f_grist.ptr = file;
	    f->f_grist.len = p - file;
	    file = p + 1;
	}

	/* Look for dir: */

	p = strrchr( file, DELIM );

	if( p )
	{
	    f->f_dir.ptr = file;
	    f->f_dir.len = p - file;
	    
	    /* Dir of : is : */
	    f->f_dir.len++;
	    file = p + 1;
	}

	end = file + strlen( file );

	/* Look for (member) */

	if( ( p = strchr( file, '(' ) ) && end[-1] == ')' )
	{
	    f->f_member.ptr = p + 1;
	    f->f_member.len = end - p - 2;
	    end = p;
	} 

	/* Look for .suffix */
	/* This would be memrchr() */

	p = 0;
	q = file;

	while( q = memchr( q, '.', end - q ) )
	    p = q++;

	if( p )
	{
	    f->f_suffix.ptr = p;
	    f->f_suffix.len = end - p;
	    end = p;
	}

	/* Leaves base */

	f->f_base.ptr = file;
	f->f_base.len = end - file;
}

/*
 * file_build() - build a filename given dir/base/suffix/member
 */
 
# define DIR_EMPTY	0	/* "" */
# define DIR_DOT	1	/* : */
# define DIR_DOTDOT	2	/* :: */
# define DIR_ABS	3	/* dira:dirb: */
# define DIR_REL	4	/* :dira:dirb: */

# define G_DIR		0	/* take dir */
# define G_ROOT		1	/* take root */
# define G_CAT		2	/* prepend root to dir */
# define G_DTDR		3	/* :: of rel dir */
# define G_DDDD		4	/* make it ::: (../..) */
# define G_MT		5	/* leave it empty */

char grid[5][5] = {
/* 		EMPTY	DOT	DOTDOT	ABS	REL */
/* EMPTY */   {	G_MT,	G_DIR,	G_DIR,	G_DIR,	G_DIR },
/* DOT */     {	G_ROOT,	G_DIR,	G_DIR,	G_DIR,	G_DIR },
/* DOTDOT */  {	G_ROOT,	G_ROOT,	G_DDDD,	G_DIR,	G_DTDR },
/* ABS */     {	G_ROOT,	G_ROOT, G_ROOT,	G_DIR,	G_CAT },
/* REL */     {	G_ROOT,	G_ROOT,	G_ROOT,	G_DIR,	G_CAT }
} ;

static int
file_flags( 
	char	*ptr,
	int	len )
{
	if( !len )
	    return DIR_EMPTY;
	if( len == 1 && ptr[0] == DELIM )
	    return DIR_DOT;
	if( len == 2 && ptr[0] == DELIM && ptr[1] == DELIM )
	    return DIR_DOTDOT;
	if( ptr[0] == DELIM )
	    return DIR_REL;
	return DIR_ABS;
}

void
file_build(
	FILENAME *f,
	string* file,
	int	binding )
{
    int dflag, rflag, act;

    file_build1( f, file );
	
    /* Combine root & directory, according to the grid. */
	
    dflag = file_flags( f->f_dir.ptr, f->f_dir.len );
    rflag = file_flags( f->f_root.ptr, f->f_root.len );
	
    switch( act = grid[ rflag ][ dflag ] )
    {
    case G_DTDR:
        {
            /* :: of rel dir */
            string_push_back( file, DELIM );
        }
        /* fall through */
		
    case G_DIR: 	
        /* take dir */
        string_append_range( file, f->f_dir.ptr, f->f_dir.ptr + f->f_dir.len  );
        break;
		
    case G_ROOT:	
        /* take root */
        string_append_range( file, f->f_root.ptr, f->f_root.ptr + f->f_root.len  );
        break;
	    
    case G_CAT:	
        /* prepend root to dir */
        string_append_range( file, f->f_root.ptr, f->f_root.ptr + f->f_root.len  );
        if( file->value[file->size - 1] == DELIM )
            string_pop_back( file );
        string_append_range( file, f->f_dir.ptr, f->f_dir.ptr + f->f_dir.len  );
        break;
	
    case G_DDDD:	
        /* make it ::: (../..) */
        string_append( file, ":::" );
        break;
    }

    /* Put : between dir and file (if none already) */
	
    if( act != G_MT && 
        file->value[file->size - 1] != DELIM && 
        ( f->f_base.len || f->f_suffix.len ) )
    {
        string_push_back( file, DELIM );
    }

    if( f->f_base.len )
    {
        string_append_range( file, f->f_base.ptr, f->f_base.ptr + f->f_base.len  );
    }

    if( f->f_suffix.len )
    {
        string_append_range( file, f->f_suffix.ptr, f->f_suffix.ptr + f->f_suffix.len  );
    }

    if( f->f_member.len )
    {
        string_push_back( file, '(' );
        string_append_range( file, f->f_member.ptr, f->f_member.ptr + f->f_member.len  );
        string_push_back( file, ')' );
    }
	
    if( DEBUG_SEARCH )
        printf(" -> '%s'\n", file->value);
}

/*
 *	file_parent() - make a FILENAME point to its parent dir
 */

void
file_parent( FILENAME *f )
{
	/* just set everything else to nothing */

	f->f_base.ptr =
	f->f_suffix.ptr =
	f->f_member.ptr = "";

	f->f_base.len = 
	f->f_suffix.len = 
	f->f_member.len = 0;
}

# endif /* OS_MAC */
