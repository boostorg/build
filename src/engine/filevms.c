/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Copyright 2005 Rene Rivera.
 *  Copyright 2015 Artur Shepilko.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */


#include "jam.h"
#include "filesys.h"

#include "object.h"
#include "pathsys.h"
#include "strings.h"


#ifdef OS_VMS

/*
 * filevms.c - manipulate file names and scan directories on VMS.
 *
 * This implementation is based on POSIX-style path manipulation.
 *
 * VMS CTRL directly supports both POSIX- and native VMS-style path expressions,
 * with the POSIX-to-VMS path translation performed internally by the same
 * set of functions. For the most part such processing is transparent, with
 * few differences mainly related to file versions (in POSIX mode only the recent
 * version is visible).
 *
 * This should allow us to re-use fileunix.c implementation,
 * excluding archive/library member processing.
 *
 * Thus in jam-files the path references can also remain POSIX/UNIX-style on all
 * levels EXCEPT in actions scope, where these must be translated to the native
 * VMS-style. This approach is somewhat similar to jam CYGWIN handling.
 *
 *
 * External routines:
 *  file_archscan()                 - scan an archive for files
 *  file_mkdir()                    - create a directory
 *  file_supported_fmt_resolution() - file modification timestamp resolution
 *
 * External routines called only via routines in filesys.c:
 *  file_collect_dir_content_() - collects directory content information
 *  file_dirscan_()             - OS specific file_dirscan() implementation
 *  file_query_()               - query information about a path from the OS
 */

#include <assert.h>
#include <stdio.h>

#include <sys/stat.h>  /* needed for mkdir() */
#include <unistd.h>  /* needed for read and close prototype */

#include <dirent.h>
#define STRUCT_DIRENT struct dirent


void path_translate_to_os_( char const * f, string * file );

/*
 * file_collect_dir_content_() - collects directory content information
 */

int file_collect_dir_content_( file_info_t * const d )
{
    LIST * files = L0;
    PATHNAME f;
    DIR * dd;
    STRUCT_DIRENT * dirent;
    string path[ 1 ];
    char const * dirstr;

    assert( d );
    assert( d->is_dir );
    assert( list_empty( d->files ) );

    dirstr = object_str( d->name );

    memset( (char *)&f, '\0', sizeof( f ) );
    f.f_dir.ptr = dirstr;
    f.f_dir.len = strlen( dirstr );

    if ( !*dirstr ) dirstr = ".";

    if ( !( dd = opendir( dirstr ) ) )
        return -1;

    string_new( path );
    while ( ( dirent = readdir( dd ) ) )
    {
        OBJECT * name;
        f.f_base.ptr = dirent->d_name
        #ifdef old_sinix
            - 2  /* Broken structure definition on sinix. */
        #endif
            ;
        f.f_base.len = strlen( f.f_base.ptr );

        string_truncate( path, 0 );
        path_build( &f, path );
        name = object_new( path->value );
        /* Immediately stat the file to preserve invariants. */
        if ( file_query( name ) )
            files = list_push_back( files, name );
        else
            object_free( name );
    }
    string_free( path );

    closedir( dd );

    d->files = files;
    return 0;
}


/*
 * file_dirscan_() - OS specific file_dirscan() implementation
 */

void file_dirscan_( file_info_t * const d, scanback func, void * closure )
{
    assert( d );
    assert( d->is_dir );

    /* Special case / : enter it */
    if ( !strcmp( object_str( d->name ), "/" ) )
        (*func)( closure, d->name, 1 /* stat()'ed */, &d->time );
}


/*
 * file_mkdir() - create a directory
 */

int file_mkdir( char const * const path )
{
    /* Explicit cast to remove const modifiers and avoid related compiler
     * warnings displayed when using the intel compiler.
     */
    return mkdir( (char *)path, 0777 );
}


/*
 * file_query_() - query information about a path from the OS
 */

void file_query_( file_info_t * const info )
{
    file_query_posix_( info );
}


/*
 * file_supported_fmt_resolution() - file modification timestamp resolution
 *
 * Returns the minimum file modification timestamp resolution supported by this
 * Boost Jam implementation. File modification timestamp changes of less than
 * the returned value might not be recognized.
 *
 * Does not take into consideration any OS or file system related restrictions.
 *
 * Return value 0 indicates that any value supported by the OS is also supported
 * here.
 */

void file_supported_fmt_resolution( timestamp * const t )
{
    /* The current implementation does not support file modification timestamp
     * resolution of less than one second.
     */
    timestamp_init( t, 1, 0 );
}

/*------------------------------------------------------------------------------
* VMS-specific processing:
*
*/

#include <descrip.h>
#include <lbrdef.h>
#include <credef.h>
#include <mhddef.h>
#include <lhidef.h>
#include <lib$routines.h>
#include <starlet.h>

/* Supply missing prototypes for lbr$-routines*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int lbr$set_module(
    void **,
    unsigned long *,
    struct dsc$descriptor_s *,
    unsigned short *,
    void * );

int lbr$open( void **,
    struct dsc$descriptor_s *,
    void *,
    void *,
    void *,
    void *,
    void * );

int lbr$ini_control(
    void **,
    unsigned long *,
    unsigned long *,
    void * );

int lbr$get_index(
    void **,
    unsigned long *,
    int (*func)( struct dsc$descriptor_s *, unsigned long *),
    void * );

int lbr$close(
    void ** );

#ifdef __cplusplus
}
#endif /* __cplusplus */



static void
file_cvttime(
    unsigned int *curtime,
    time_t *unixtime )
{
    static const size_t divisor = 10000000;
    static unsigned int bastim[2] = { 0x4BEB4000, 0x007C9567 }; /* 1/1/1970 */
    int delta[2], remainder;

    lib$subx( curtime, bastim, delta );
    lib$ediv( &divisor, delta, unixtime, &remainder );
}



static const char *VMS_archive = 0;
static scanback VMS_func;
static void *VMS_closure;
static void *context;

static int
file_archmember(
    struct dsc$descriptor_s *module,
    unsigned long *rfa )
{
    static struct dsc$descriptor_s bufdsc =
          {0, DSC$K_DTYPE_T, DSC$K_CLASS_S, NULL};

    struct mhddef *mhd;
    char filename[128] = { 0 };
    char buf[ MAXJPATH ] = { 0 };

    int status;
    time_t library_date;

    register int i;
    register char *p;

    OBJECT * member;

    bufdsc.dsc$a_pointer = filename;
    bufdsc.dsc$w_length = sizeof( filename );
    status = lbr$set_module( &context, rfa, &bufdsc,
                 &bufdsc.dsc$w_length, NULL );

    if ( !(status & 1) )
    return ( 1 );

    mhd = (struct mhddef *)filename;

    file_cvttime( &mhd->mhd$l_datim, &library_date );

    for ( i = 0, p = module->dsc$a_pointer; i < module->dsc$w_length; ++i, ++p )
    filename[ i ] = *p;

    filename[ i ] = '\0';

    sprintf( buf, "%s(%s.obj)", VMS_archive, filename );

    member = object_new( buf );
    (*VMS_func)( VMS_closure, member, 1 /* time valid */, (time_t)library_date );
    object_free( member );

    return ( 1 );
}


void file_archscan( const char * archive, scanback func, void * closure )
{
    static struct dsc$descriptor_s library =
          {0, DSC$K_DTYPE_T, DSC$K_CLASS_S, NULL};

    unsigned long lfunc = LBR$C_READ;
    unsigned long typ = LBR$C_TYP_UNK;
    unsigned long index = 1;

    register int status;
    string buf[ 1 ];
    char vmspath[ MAXJPATH ] = { 0 };

    VMS_archive = archive;
    VMS_func = func;
    VMS_closure = closure;


    /*  Translate path to VMS
     */
    string_new( buf );
    path_translate_to_os_( archive, buf );
    strcpy( vmspath, buf->value );
    string_free( buf );

    status = lbr$ini_control( &context, &lfunc, &typ, NULL );
    if ( !( status & 1 ) )
        return;

    library.dsc$a_pointer = vmspath;
    library.dsc$w_length = strlen( vmspath );

    status = lbr$open( &context, &library, NULL, NULL, NULL, NULL, NULL );
    if ( !( status & 1 ) )
        return;

    (void) lbr$get_index( &context, &index, file_archmember, NULL );

    (void) lbr$close( &context );
}

#endif  /* OS_VMS */

