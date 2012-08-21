/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/* This file is ALSO:
 * Copyright 2001-2004 David Abrahams.
 * Copyright 2005 Rene Rivera.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * pathunix.c - manipulate file names on UNIX, NT, OS2, AmigaOS
 *
 * External routines:
 *  path_parse() - split a file name into dir/base/suffix/member
 *  path_build() - build a filename given dir/base/suffix/member
 *  path_parent() - make a PATHNAME point to its parent dir
 *
 * File_parse() and path_build() just manipulate a string and a structure;
 * they do not make system calls.
 */

#include "jam.h"

#ifdef USE_PATHUNIX

#include "pathsys.h"

#include "filesys.h"
#include "object.h"
#include "strings.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#ifndef OS_NT
# include <unistd.h>
#endif


/*
 * path_parse() - split a file name into dir/base/suffix/member
 */

void path_parse( char const * file, PATHNAME * f )
{
    char const * p;
    char const * q;
    char const * end;

    memset( (char *)f, 0, sizeof( *f ) );

    /* Look for '<grist>'. */

    if ( ( file[ 0 ] == '<' ) && ( p = strchr( file, '>' ) ) )
    {
        f->f_grist.ptr = file;
        f->f_grist.len = p - file;
        file = p + 1;
    }

    /* Look for 'dir/'. */

    p = strrchr( file, '/' );

#if PATH_DELIM == '\\'
    /* On NT, look for dir\ as well */
    {
        char * const p1 = strrchr( file, '\\' );
        p = p1 > p ? p1 : p;
    }
#endif

    if ( p )
    {
        f->f_dir.ptr = file;
        f->f_dir.len = p - file;

        /* Special case for / - dirname is /, not "" */
        if ( !f->f_dir.len )
            ++f->f_dir.len;

#if PATH_DELIM == '\\'
        /* Special case for D:/ - dirname is D:/, not "D:" */
        if ( f->f_dir.len == 2 && file[ 1 ] == ':' )
            ++f->f_dir.len;
#endif

        file = p + 1;
    }

    end = file + strlen( file );

    /* Look for '(member)'. */
    if ( ( p = strchr( file, '(' ) ) && ( end[ -1 ] == ')' ) )
    {
        f->f_member.ptr = p + 1;
        f->f_member.len = end - p - 2;
        end = p;
    }

    /* Look for '.suffix'. This would be memrchr(). */
    p = 0;
    for ( q = file; ( q = (char *)memchr( q, '.', end - q ) ); ++q )
        p = q;
    if ( p )
    {
        f->f_suffix.ptr = p;
        f->f_suffix.len = end - p;
        end = p;
    }

    /* Leaves base. */
    f->f_base.ptr = file;
    f->f_base.len = end - file;
}


/*
 * path_delims - the string of legal path delimiters
 */

static char path_delims[] = {
    PATH_DELIM,
#if PATH_DELIM == '\\'
    '/',
#endif
    0
};


/*
 * is_path_delim() - true iff c is a path delimiter
 */

static int is_path_delim( char const c )
{
    char const * const p = strchr( path_delims, c );
    return p && *p;
}


/*
 * as_path_delim() - convert c to a path delimiter if it is not one already
 */

static char as_path_delim( char const c )
{
    return is_path_delim( c ) ? c : PATH_DELIM;
}


/*
 * path_build() - build a filename given dir/base/suffix/member
 *
 * To avoid changing slash direction on NT when reconstituting paths, instead of
 * unconditionally appending PATH_DELIM we check the past-the-end character of
 * the previous path element. If it is in path_delims, we append that, and only
 * append PATH_DELIM as a last resort. This heuristic is based on the fact that
 * PATHNAME objects are usually the result of calling path_parse, which leaves
 * the original slashes in the past-the-end position. Correctness depends on the
 * assumption that all strings are zero terminated, so a past-the-end character
 * will always be available.
 *
 * As an attendant patch, we had to ensure that backslashes are used explicitly
 * in 'timestamp.c'.
 */

void path_build( PATHNAME * f, string * file )
{
    file_build1( f, file );

    /* Do not prepend root if it is '.' or the directory is rooted. */
    if ( f->f_root.len
        && !( f->f_root.len == 1 && f->f_root.ptr[ 0 ] == '.' )
        && !( f->f_dir.len && f->f_dir.ptr[ 0 ] == '/' )
#if PATH_DELIM == '\\'
        && !( f->f_dir.len && f->f_dir.ptr[ 0 ] == '\\' )
        && !( f->f_dir.len && f->f_dir.ptr[ 1 ] == ':' )
#endif
    )
    {
        string_append_range( file, f->f_root.ptr, f->f_root.ptr + f->f_root.len
            );
        /* If 'root' already ends with a path delimeter, do not add another one.
         */
        if ( !is_path_delim( f->f_root.ptr[ f->f_root.len - 1 ] ) )
            string_push_back( file, as_path_delim( f->f_root.ptr[ f->f_root.len
                ] ) );
    }

    if ( f->f_dir.len )
        string_append_range( file, f->f_dir.ptr, f->f_dir.ptr + f->f_dir.len );

    /* Put path separator between dir and file. */
    /* Special case for root dir: do not add another path separator. */
    if ( f->f_dir.len && ( f->f_base.len || f->f_suffix.len )
#if PATH_DELIM == '\\'
        && !( f->f_dir.len == 3 && f->f_dir.ptr[ 1 ] == ':' )
#endif
        && !( f->f_dir.len == 1 && is_path_delim( f->f_dir.ptr[ 0 ] ) ) )
        string_push_back( file, as_path_delim( f->f_dir.ptr[ f->f_dir.len ] ) );

    if ( f->f_base.len )
        string_append_range( file, f->f_base.ptr, f->f_base.ptr + f->f_base.len
            );

    if ( f->f_suffix.len )
        string_append_range( file, f->f_suffix.ptr, f->f_suffix.ptr +
            f->f_suffix.len );

    if ( f->f_member.len )
    {
        string_push_back( file, '(' );
        string_append_range( file, f->f_member.ptr, f->f_member.ptr +
            f->f_member.len );
        string_push_back( file, ')' );
    }
}


/*
 *  path_parent() - make a PATHNAME point to its parent dir
 */

void path_parent( PATHNAME * f )
{
    /* Just clear everything. */

    f->f_base.ptr =
    f->f_suffix.ptr =
    f->f_member.ptr = "";

    f->f_base.len =
    f->f_suffix.len =
    f->f_member.len = 0;
}

#ifdef NT
#include <windows.h>

/* The definition of this in winnt.h is not ANSI-C compatible. */
#undef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)


typedef struct path_key_entry
{
    OBJECT * path;
    OBJECT * key;
} path_key_entry;

static struct hash * path_key_cache;


/*
 * canonicWindowsPath() - convert a given path into its canonic/long format
 *
 * Appends the canonic path to the end of the given 'string' object.
 *
 * FIXME: This function is still work-in-progress as it originally did not
 * necessarily return the canonic path format (could return slightly different
 * results for certain equivalent path strings) and could accept paths pointing
 * to non-existing file system entities as well.
 *
 * Caches results internally, automatically caching any parent paths it has to
 * convert to their canonic format in the process.
 *
 * Prerequisites:
 *  - path given in normalized form, i.e. all of its folder separators have
 *    already been converted into '\\'
 *  - path_key_cache path/key mapping cache object already initialized
 */

static void canonicWindowsPath( char const * const path, int const path_length,
    string * const out )
{
    char const * last_element;
    unsigned long saved_size;
    char const * p;

    /* This is only called via path_key(), which initializes the cache. */
    assert( path_key_cache );

    if ( !path_length )
        return;

    if ( path_length == 1 && path[ 0 ] == '\\' )
    {
        string_push_back( out, '\\' );
        return;
    }

    if ( path[ 1 ] == ':' &&
        ( path_length == 2 ||
        ( path_length == 3 && path[ 2 ] == '\\' ) ) )
    {
        string_push_back( out, toupper( path[ 0 ] ) );
        string_push_back( out, ':' );
        string_push_back( out, '\\' );
        return;
    }

    /* Find last '\\'. */
    for ( p = path + path_length - 1; p >= path && *p != '\\'; --p );
    last_element = p + 1;

    /* Special case '\' && 'D:\' - include trailing '\'. */
    if ( p == path ||
        p == path + 2 && path[ 1 ] == ':' )
        ++p;

    if ( p >= path )
    {
        char const * const dir = path;
        int const dir_length = p - path;
        OBJECT * const dir_obj = object_new_range( dir, dir_length );
        int found;
        path_key_entry * const result = (path_key_entry *)hash_insert(
            path_key_cache, dir_obj, &found );
        if ( !found )
        {
            result->path = dir_obj;
            canonicWindowsPath( dir, dir_length, out );
            result->key = object_new( out->value );
        }
        else
        {
            object_free( dir_obj );
            string_append( out, object_str( result->key ) );
        }
    }

    if ( out->size && out->value[ out->size - 1 ] != '\\' )
        string_push_back( out, '\\' );

    saved_size = out->size;
    string_append_range( out, last_element, path + path_length );

    {
        char const * const n = last_element;
        int const n_length = path + path_length - n;
        if ( !( n_length == 1 && n[ 0 ] == '.' )
            && !( n_length == 2 && n[ 0 ] == '.' && n[ 1 ] == '.' ) )
        {
            WIN32_FIND_DATA fd;
            HANDLE const hf = FindFirstFileA( out->value, &fd );
            if ( hf != INVALID_HANDLE_VALUE )
            {
                string_truncate( out, saved_size );
                string_append( out, fd.cFileName );
                FindClose( hf );
            }
        }
    }
}


/*
 * normalize_path() - 'normalizes' the given path for the path-key mapping
 *
 * The resulting string has nothing to do with 'normalized paths' as used in
 * Boost Jam build scripts and the built-in NORMALIZE_PATH rule. It is intended
 * to be used solely as an intermediate step when mapping an arbitrary path to
 * its canonical representation.
 *
 * When choosing the intermediate string the important things are for it to be
 * inexpensive to calculate and any two paths having different canonical
 * representations also need to have different calculated intermediate string
 * representations. Any implemented additional rules serve only to simplify
 * constructing the canonical path representation from the calculated
 * intermediate string.
 *
 * Implemented returned path rules:
 *  - use backslashes as path separators
 *  - lowercase only (since all Windows file systems are case insensitive)
 *  - trim trailing path separator except in case of a root path, i.e. 'X:\'
 */

static void normalize_path( string * path )
{
    char * s;
    for ( s = path->value; s < path->value + path->size; ++s )
        *s = *s == '/' ? '\\' : tolower( *s );
    /* Strip trailing "/". */
    if ( path->size && path->size != 3 && path->value[ path->size - 1 ] == '\\'
        )
        string_pop_back( path );
}


static path_key_entry * path_key( OBJECT * const path,
    int const known_to_be_canonic )
{
    path_key_entry * result;
    int found;

    if ( !path_key_cache )
        path_key_cache = hashinit( sizeof( path_key_entry ), "path to key" );

    result = (path_key_entry *)hash_insert( path_key_cache, path, &found );
    if ( !found )
    {
        OBJECT * normalized;
        int normalized_size;
        path_key_entry * nresult;
        result->path = path;
        {
            string buf[ 1 ];
            string_copy( buf, object_str( path ) );
            normalize_path( buf );
            normalized = object_new( buf->value );
            normalized_size = buf->size;
            string_free( buf );
        }
        nresult = (path_key_entry *)hash_insert( path_key_cache, normalized,
            &found );
        if ( !found || nresult == result )
        {
            nresult->path = normalized;
            if ( known_to_be_canonic )
                nresult->key = object_copy( path );
            else
            {
                string canonic_path[ 1 ];
                string_new( canonic_path );
                canonicWindowsPath( object_str( normalized ), normalized_size,
                    canonic_path );
                nresult->key = object_new( canonic_path->value );
                string_free( canonic_path );
            }
        }
        else
            object_free( normalized );
        if ( nresult != result )
        {
            result->path = object_copy( path );
            result->key = object_copy( nresult->key );
        }
    }

    return result;
}


void path_register_key( OBJECT * canonic_path )
{
    path_key( canonic_path, 1 );
}


OBJECT * path_as_key( OBJECT * path )
{
    return object_copy( path_key( path, 0 )->key );
}


static void free_path_key_entry( void * xentry, void * const data )
{
    path_key_entry * const entry = (path_key_entry *)xentry;
    object_free( entry->path );
    object_free( entry->key );
}


void path_done( void )
{
    if ( path_key_cache )
    {
        hashenumerate( path_key_cache, &free_path_key_entry, (void *)0 );
        hashdone( path_key_cache );
    }
}

#else  /* NT */


void path_register_key( OBJECT * path )
{
}


OBJECT * path_as_key( OBJECT * path )
{
    return object_copy( path );
}


void path_done( void )
{
}

#endif  /* NT */


string const * path_tmpdir()
{
    static string buffer[ 1 ];
    static int have_result;
    if ( !have_result )
    {
        string_new( buffer );
        #ifdef OS_NT
        {
            DWORD pathLength = GetTempPathA( 0, NULL );
            string_reserve( buffer, pathLength );
            pathLength = GetTempPathA( pathLength, buffer->value );
            buffer->value[ pathLength - 1 ] = '\0';
            buffer->size = pathLength - 1;
        }
        #else
        {
            char const * t = getenv( "TMPDIR" );
            if ( !t ) t = "/tmp";
            string_append( buffer, t );
        }
        #endif
        have_result = 1;
    }
    return buffer;
}


OBJECT * path_tmpnam( void )
{
    char name_buffer[ 64 ];
    unsigned long const c0 =
    #ifdef OS_NT
        GetCurrentProcessId();
    #else
        getpid();
    #endif
    static unsigned long c1;
    if ( !c1 ) c1 = time( 0 ) & 0xffff;
    c1 += 1;
    sprintf( name_buffer, "jam%lx%lx.000", c0, c1 );
    return object_new( name_buffer );
}


OBJECT * path_tmpfile( void )
{
    OBJECT * result;
    OBJECT * tmpnam;

    string file_path[ 1 ];
    string_copy( file_path, path_tmpdir()->value );
    string_push_back( file_path, PATH_DELIM );
    tmpnam = path_tmpnam();
    string_append( file_path, object_str( tmpnam ) );
    object_free( tmpnam );
    result = object_new( file_path->value );
    string_free( file_path );

    return result;
}


#endif  /* USE_PATHUNIX */
