/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

# include "jam.h"
# include "filesys.h"
# include "strings.h"

# ifdef USE_PATHUNIX

/*
 * pathunix.c - manipulate file names on UNIX, NT, OS2, AmigaOS
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

	/* Look for dir/ */

	p = strrchr( file, '/' );

# ifndef UNIX
# ifndef AMIGA
	/* On NT, look for dir\ as well */
	{
	    char *p1 = strrchr( file, '\\' );
	    p = p1 > p ? p1 : p;
	}
# endif
# endif

	if( p )
	{
	    f->f_dir.ptr = file;
	    f->f_dir.len = p - file;
	
	    /* Special case for / - dirname is /, not "" */

	    if( !f->f_dir.len )
		f->f_dir.len = 1;

# ifndef UNIX
# ifndef AMIGA
	    /* Special case for D:/ - dirname is D:/, not "D:" */

	    if( f->f_dir.len == 2 && file[1] == ':' )
		f->f_dir.len = 3;
# endif
# endif

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

	while( q = (char *)memchr( q, '.', end - q ) )
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

void
file_build(
	FILENAME *f,
	string	*file,
	int	binding )
{
    file_build1( f, file );
    
    /* Don't prepend root if it's . or directory is rooted */
# if PATH_DELIM == '/'

    if( f->f_root.len 
        && !( f->f_root.len == 1 && f->f_root.ptr[0] == '.' )
        && !( f->f_dir.len && f->f_dir.ptr[0] == '/' ) )

# else /* unix */

    if( f->f_root.len 
        && !( f->f_root.len == 1 && f->f_root.ptr[0] == '.' )
        && !( f->f_dir.len && f->f_dir.ptr[0] == '/' )
        && !( f->f_dir.len && f->f_dir.ptr[0] == '\\' )
        && !( f->f_dir.len && f->f_dir.ptr[1] == ':' ) )

# endif /* unix */

    {
        string_append_range( file, f->f_root.ptr, f->f_root.ptr + f->f_root.len  );
        string_push_back( file, PATH_DELIM );
    }

    if( f->f_dir.len )
    {
        string_append_range( file, f->f_dir.ptr, f->f_dir.ptr + f->f_dir.len  );
    }

    /* UNIX: Put / between dir and file */
    /* NT:   Put \ between dir and file */

    if( f->f_dir.len && ( f->f_base.len || f->f_suffix.len ) )
    {
        /* UNIX: Special case for dir \ : don't add another \ */
        /* NT:   Special case for dir / : don't add another / */

# if PATH_DELIM == '\\'
        if( !( f->f_dir.len == 3 && f->f_dir.ptr[1] == ':' ) )
# endif
            if( !( f->f_dir.len == 1 && f->f_dir.ptr[0] == PATH_DELIM ) )
                string_push_back( file, PATH_DELIM );
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

# endif /* unix, NT, OS/2, AmigaOS */
