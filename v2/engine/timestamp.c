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
 * timestamp.c - get the timestamp of a file or archive member
 *
 * External routines:
 *  timestamp_from_target() - return timestamp on a file, if present
 *  timestamp_done()        - free timestamp tables
 *
 * Internal routines:
 *  time_enter()      - internal worker callback for scanning archives &
 *                      directories
 *  free_timestamps() - worker function for freeing timestamp table contents
 */

#include "jam.h"
#include "timestamp.h"

#include "filesys.h"
#include "hash.h"
#include "object.h"
#include "pathsys.h"
#include "strings.h"


/*
 * BINDING - all known files
 */

typedef struct _binding {
    OBJECT * name;
    short    flags;

#define BIND_SCANNED  0x01  /* if directory or arch, has been scanned */

    short    progress;

#define BIND_INIT     0  /* never seen */
#define BIND_NOENTRY  1  /* timestamp requested but file never found */
#define BIND_SPOTTED  2  /* file found but not timed yet */
#define BIND_MISSING  3  /* file found but can not get timestamp */
#define BIND_FOUND    4  /* file found and time stamped */

    /* update time - 0 if not exist */
    time_t   time;
} BINDING;

static struct hash * bindhash = 0;
static void time_enter( void *, OBJECT *, int const found, time_t );

static char * time_progress[] =
{
    "INIT",
    "NOENTRY",
    "SPOTTED",
    "MISSING",
    "FOUND"
};


/*
 * timestamp_from_target() - return timestamp on a file, if present
 */

void timestamp_from_target( OBJECT * target, time_t * time )
{
    PROFILE_ENTER( timestamp );

    PATHNAME f1;
    PATHNAME f2;
    int found;
    BINDING * b;
    string buf[ 1 ];

    target = path_as_key( target );

    string_new( buf );

    if ( !bindhash )
        bindhash = hashinit( sizeof( BINDING ), "bindings" );

    /* Quick path - is it there? */

    b = (BINDING *)hash_insert( bindhash, target, &found );
    if ( !found )
    {
        b->name = object_copy( target );  /* never freed */
        b->time = b->flags = 0;
        b->progress = BIND_INIT;
    }

    if ( b->progress != BIND_INIT )
        goto afterscanning;

    b->progress = BIND_NOENTRY;

    /* Not found - have to scan for it. */
    path_parse( object_str( target ), &f1 );

    /* Scan directory if not already done so. */
    {
        int found;
        BINDING * b;
        OBJECT * name;

        f2 = f1;
        f2.f_grist.len = 0;
        path_parent( &f2 );
        path_build( &f2, buf, 0 );

        name = object_new( buf->value );

        b = (BINDING *)hash_insert( bindhash, name, &found );
        if ( !found )
        {
            b->name = object_copy( name );
            b->time = b->flags = 0;
            b->progress = BIND_INIT;
        }

        if ( !( b->flags & BIND_SCANNED ) )
        {
            file_dirscan( name, time_enter, bindhash );
            b->flags |= BIND_SCANNED;
        }

        object_free( name );
    }

    /* Scan archive if not already done so. */
    if ( f1.f_member.len )
    {
        int found;
        BINDING * b;
        OBJECT * name;

        f2 = f1;
        f2.f_grist.len = 0;
        f2.f_member.len = 0;
        string_truncate( buf, 0 );
        path_build( &f2, buf, 0 );

        name = object_new( buf->value );

        b = (BINDING *)hash_insert( bindhash, name, &found );
        if ( !found )
        {
            b->name = object_copy( name );
            b->time = b->flags = 0;
            b->progress = BIND_INIT;
        }

        if ( !( b->flags & BIND_SCANNED ) )
        {
            file_archscan( buf->value, time_enter, bindhash );
            b->flags |= BIND_SCANNED;
        }

        object_free( name );
    }

    afterscanning:

    if ( b->progress == BIND_SPOTTED )
    {
         b->progress = file_time( b->name, &b->time ) < 0
            ? BIND_MISSING
            : BIND_FOUND;
    }

    *time = b->progress == BIND_FOUND ? b->time : 0;

    string_free( buf );
    object_free( target );

    PROFILE_EXIT( timestamp );
}


/*
 * time_enter() - internal worker callback for scanning archives & directories
 */

static void time_enter( void * closure, OBJECT * target, int const found,
    time_t time )
{
    int item_found;
    BINDING * b;
    struct hash * const bindhash = (struct hash *)closure;

    target = path_as_key( target );

    b = (BINDING *)hash_insert( bindhash, target, &item_found );
    if ( !item_found )
    {
        b->name = object_copy( target );
        b->flags = 0;
    }

    b->time = time;
    b->progress = found ? BIND_FOUND : BIND_SPOTTED;

    if ( DEBUG_BINDSCAN )
        printf( "time ( %s ) : %s\n", object_str( target ), time_progress[
            b->progress ] );

    object_free( target );
}


/*
 * free_timestamps() - worker function for freeing timestamp table contents
 */

static void free_timestamps( void * xbinding, void * data )
{
    object_free( ((BINDING *)xbinding)->name );
}


/*
 * timestamp_done() - free timestamp tables
 */

void timestamp_done()
{
    if ( bindhash )
    {
        hashenumerate( bindhash, free_timestamps, (void *)0 );
        hashdone( bindhash );
    }
}
