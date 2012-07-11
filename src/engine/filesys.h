/*
 * Copyright 1993-2002 Christopher Seiwald and Perforce Software, Inc.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * filesys.h - OS specific file routines
 */

#ifndef FILESYS_DWA20011025_H
#define FILESYS_DWA20011025_H

#include "hash.h"
#include "lists.h"
#include "object.h"
#include "pathsys.h"


typedef struct file_info_t
{
    OBJECT * name;
    short is_file;
    short is_dir;
    unsigned long size;
    time_t time;
    LIST * files;
} file_info_t;

typedef void (*scanback)( void * closure, OBJECT * path, int found, time_t t );


void file_archscan( char const * arch, scanback func, void * closure );
void file_build1( PATHNAME * f, string * file ) ;
void file_dirscan( OBJECT * dir, scanback func, void * closure );
file_info_t * file_info( OBJECT * path );
int file_is_file( OBJECT * path );
int file_mkdir( char const * const path );
file_info_t * file_query( OBJECT * path );
void file_remove_atexit( OBJECT * const path );
int file_time( OBJECT * path, time_t * time );

/* Internal utility worker functions. */
int file_query_posix_( file_info_t * const info );

void file_done();

#endif
