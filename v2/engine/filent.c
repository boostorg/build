/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Copyright 2005 Rene Rivera.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * filent.c - scan directories and archives on NT
 *
 * External routines:
 *  file_dirscan() - scan a directory for files
 *  file_time() - get timestamp of file, if not done by file_dirscan()
 *  file_archscan() - scan an archive for files
 *
 * File_dirscan() and file_archscan() call back a caller provided function for
 * each file found. A flag to this callback function lets file_dirscan() and
 * file_archscan() indicate whether a timestamp is being provided with the file.
 * If file_dirscan() or file_archscan() do not provide the file's timestamp,
 * interested parties may later call file_time() for it.
 */

#include "jam.h"

#ifdef OS_NT

#include "filesys.h"

#include "object.h"
#include "pathsys.h"
#include "strings.h"

#ifdef __BORLANDC__
# if __BORLANDC__ < 0x550
#  include <dir.h>
#  include <dos.h>
# endif
# undef FILENAME  /* cpp namespace collision */
# define _finddata_t ffblk
#endif

#include <ctype.h>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>

#include <windows.h>


/*
 * file_dirscan() - scan a directory for files
 */

void file_dirscan( OBJECT * dir, scanback func, void * closure )
{
    PROFILE_ENTER( FILE_DIRSCAN );

    /* First enter the directory itself. */

    file_info_t * const d = file_query( dir );
    if ( !d || !d->is_dir )
    {
        PROFILE_EXIT( FILE_DIRSCAN );
        return;
    }

    if ( !d->files )
    {
        PATHNAME f;
        string filespec[ 1 ];
        string filename[ 1 ];
        struct _finddata_t finfo[ 1 ];
        LIST * files = L0;
        OBJECT * const long_dir = short_path_to_long_path( dir );
        int const d_length = strlen( object_str( long_dir ) );

        memset( (char *)&f, '\0', sizeof( f ) );

        /* Now enter contents of directory */

        /* Prepare file search specification for the findfirst() API. */
        if ( !d_length )
            string_copy( filespec, ".\\*" );
        else
        {
            /* We can not simply assume the given folder name will never include
             * its trailing path separator or otherwise we would not support the
             * Windows root folder specified without its drive letter, i.e. '\'.
             */
            char const trailingChar = object_str( long_dir )[ d_length - 1 ] ;
            string_copy( filespec, object_str( long_dir ) );
            if ( ( trailingChar != '\\' ) && ( trailingChar != '/' ) )
                string_append( filespec, "\\" );
            string_append( filespec, "*" );
        }

        if ( DEBUG_BINDSCAN )
            printf( "scan directory %s\n", long_dir );

        object_free( long_dir );

        #if defined(__BORLANDC__) && __BORLANDC__ < 0x550
        if ( findfirst( filespec->value, finfo, FA_NORMAL | FA_DIREC ) )
        {
            string_free( filespec );
            PROFILE_EXIT( FILE_DIRSCAN );
            return;
        }

        string_new( filename );
        do
        {
            f.f_base.ptr = finfo->ff_name;
            f.f_base.len = strlen( finfo->ff_name );
            string_truncate( filename, 0 );
            path_build( &f, filename );

            files = list_push_back( files, object_new( filename->value ) );
            {
                file_info_t * const ff = file_info( filename->value );
                ff->is_file = finfo->ff_attrib & FA_DIREC ? 0 : 1;
                ff->is_dir = !ff->is_file;
                ff->size = finfo->ff_fsize;
                ff->time = ( finfo->ff_ftime << 16 ) | finfo->ff_ftime;
            }
        }
        while ( !findnext( finfo ) );
        #else
        {
            long const handle = _findfirst( filespec->value, finfo );
            if ( handle < 0L )
            {
                string_free( filespec );
                PROFILE_EXIT( FILE_DIRSCAN );
                return;
            }

            string_new( filename );
            do
            {
                OBJECT * filename_obj;

                f.f_base.ptr = finfo->name;
                f.f_base.len = strlen( finfo->name );
                string_truncate( filename, 0 );
                path_build( &f, filename, 0 );

                filename_obj = object_new( filename->value );
                path_key__register_long_path( filename_obj );
                files = list_push_back( files, filename_obj );
                {
                    file_info_t * const ff = file_info( filename_obj );
                    ff->is_file = finfo->attrib & _A_SUBDIR ? 0 : 1;
                    ff->is_dir = !ff->is_file;
                    ff->size = finfo->size;
                    ff->time = finfo->time_write;
                }
            }
            while ( !_findnext( handle, finfo ) );

            _findclose( handle );
        }
        #endif
        string_free( filename );
        string_free( filespec );

        d->files = files;
    }

    /* Special case \ or d:\ : enter it */
    {
        unsigned long len = strlen( object_str( d->name ) );
        if ( len == 1 && object_str( d->name )[ 0 ] == '\\' )
        {
            OBJECT * const long_dir = short_path_to_long_path( d->name );
            (*func)( closure, long_dir, 1 /* stat()'ed */, d->time );
            object_free( long_dir );
        }
        else if ( len == 3 && object_str( d->name )[ 1 ] == ':' )
        {
            /* We have just entered a 3-letter drive name spelling (with a
             * trailing slash), into the hash table. Now enter its two-letter
             * variant, without the trailing slash, so that if we try to check
             * whether "c:" exists, we hit it.
             *
             * Jam core has workarounds for that. Given:
             *    x = c:\whatever\foo ;
             *    p = $(x:D) ;
             *    p2 = $(p:D) ;
             * There will be no trailing slash in $(p), but there will be one in
             * $(p2). But, that seems rather fragile.
             */
            OBJECT * const dir1 = short_path_to_long_path( d->name );
            OBJECT * dir2;
            char buf[ 3 ];
            buf[ 0 ] = object_str( dir1 )[ 0 ];
            buf[ 1 ] = object_str( dir1 )[ 1 ];
            buf[ 2 ] = 0;
            dir2 = object_new( buf );
            (*func)( closure, dir1, 1 /* stat()'ed */, d->time );
            (*func)( closure, dir2, 1 /* stat()'ed */, d->time );
            object_free( dir2 );
            object_free( dir1 );
        }
    }

    /* Now enter contents of directory */
    if ( !list_empty( d->files ) )
    {
        LIST * const files = d->files;
        LISTITER iter = list_begin( files );
        LISTITER const end = list_end( files );
        for ( ; iter != end; iter = list_next( iter ) )
        {
            file_info_t const * const ff = file_info( list_item( iter ) );
            (*func)( closure, list_item( iter ), 1 /* stat()'ed */, ff->time );
        }
    }

    PROFILE_EXIT( FILE_DIRSCAN );
}


file_info_t * file_query( OBJECT * filename )
{
    file_info_t * const ff = file_info( filename );
    if ( !ff->time )
    {
        char const * const name = *object_str( filename )
            ? object_str( filename )
            : ".";

        /* POSIX stat() function on Windows suffers from several issues:
         *   * Does not support file timestamps with resolution finer than 1
         *     second. This means it can not be used to detect file timestamp
         *     changes of less than one second. One possible consequence is that
         *     some fast-pased touch commands (such as those done by Boost
         *     Build's internal testing system if it does not do extra waiting
         *     before those touch operations) will not be detected correctly by
         *     the build system.
         *   * Returns file modification times automatically adjusted for
         *     daylight savings time even though daylight savings time should
         *     have nothing to do with internal time representation.
         */
        struct stat statbuf;

        if ( stat( name, &statbuf ) < 0 )
            return 0;

        ff->is_file = statbuf.st_mode & S_IFREG ? 1 : 0;
        ff->is_dir = statbuf.st_mode & S_IFDIR ? 1 : 0;
        ff->size = statbuf.st_size;
        /* Set the file's timestamp to 1 in case it is 0 or undetected to avoid
         * confusion with non-existing files.
         */
        ff->time = statbuf.st_mtime ? statbuf.st_mtime : 1;
    }
    return ff;
}


/*
 * file_time() - get timestamp of file, if not done by file_dirscan()
 */

int file_time( OBJECT * filename, time_t * time )
{
    file_info_t const * const ff = file_query( filename );
    if ( !ff ) return -1;
    *time = ff->time;
    return 0;
}


int file_is_file( OBJECT * filename )
{
    file_info_t const * const ff = file_query( filename );
    return ff ? ff->is_file : -1;
}


int file_mkdir( char const * pathname )
{
    return _mkdir( pathname );
}


/*
 * file_archscan() - scan an archive for files
 */

/* Straight from SunOS */

#define ARMAG   "!<arch>\n"
#define SARMAG  8

#define ARFMAG  "`\n"

struct ar_hdr {
    char ar_name[ 16 ];
    char ar_date[ 12 ];
    char ar_uid[ 6 ];
    char ar_gid[ 6 ];
    char ar_mode[ 8 ];
    char ar_size[ 10 ];
    char ar_fmag[ 2 ];
};

#define SARFMAG 2
#define SARHDR sizeof( struct ar_hdr )

void file_archscan( char const * archive, scanback func, void * closure )
{
    struct ar_hdr ar_hdr;
    char * string_table = 0;
    char buf[ MAXJPATH ];
    long offset;
    int const fd = open( archive, O_RDONLY | O_BINARY, 0 );

    if ( fd < 0 )
        return;

    if ( read( fd, buf, SARMAG ) != SARMAG || strncmp( ARMAG, buf, SARMAG ) )
    {
        close( fd );
        return;
    }

    offset = SARMAG;

    if ( DEBUG_BINDSCAN )
        printf( "scan archive %s\n", archive );

    while ( ( read( fd, &ar_hdr, SARHDR ) == SARHDR ) &&
        !memcmp( ar_hdr.ar_fmag, ARFMAG, SARFMAG ) )
    {
        long lar_date;
        long lar_size;
        char * name = 0;
        char * endname;
        char * c;
        OBJECT * member;

        sscanf( ar_hdr.ar_date, "%ld", &lar_date );
        sscanf( ar_hdr.ar_size, "%ld", &lar_size );

        lar_size = ( lar_size + 1 ) & ~1;

        if ( ar_hdr.ar_name[ 0 ] == '/' && ar_hdr.ar_name[ 1 ] == '/' )
        {
            /* This is the "string table" entry of the symbol table, holding
             * filename strings longer than 15 characters, i.e. those that do
             * not fit into ar_name.
             */
            string_table = BJAM_MALLOC_ATOMIC( lar_size + 1 );
            if ( read( fd, string_table, lar_size ) != lar_size )
                printf( "error reading string table\n" );
            string_table[ lar_size ] = '\0';
            offset += SARHDR + lar_size;
            continue;
        }
        else if ( ar_hdr.ar_name[ 0 ] == '/' && ar_hdr.ar_name[ 1 ] != ' ' )
        {
            /* Long filenames are recognized by "/nnnn" where nnnn is the
             * string's offset in the string table represented in ASCII
             * decimals.
             */
            name = string_table + atoi( ar_hdr.ar_name + 1 );
            for ( endname = name; *endname && *endname != '\n'; ++endname );
        }
        else
        {
            /* normal name */
            name = ar_hdr.ar_name;
            endname = name + sizeof( ar_hdr.ar_name );
        }

        /* strip trailing white-space, slashes, and backslashes */

        while ( endname-- > name )
            if ( !isspace( *endname ) && ( *endname != '\\' ) && ( *endname !=
                '/' ) )
                break;
        *++endname = 0;

        /* strip leading directory names, an NT specialty */

        if ( c = strrchr( name, '/' ) )
            name = c + 1;
        if ( c = strrchr( name, '\\' ) )
            name = c + 1;

        sprintf( buf, "%s(%.*s)", archive, endname - name, name );
        member = object_new( buf );
        (*func)( closure, member, 1 /* time valid */, (time_t)lar_date );
        object_free( member );

        offset += SARHDR + lar_size;
        lseek( fd, offset, 0 );
    }

    close( fd );
}

#endif  /* OS_NT */
