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
 *  timestamp_from_path() - return timestamp for a path, if present
 *  timestamp_done()      - free timestamp tables
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
    short flags;

#define BIND_SCANNED  0x01  /* if directory or arch, has been scanned */

    short progress;

#define BIND_INIT     0  /* never seen */
#define BIND_NOENTRY  1  /* timestamp requested but file never found */
#define BIND_SPOTTED  2  /* file found but not timed yet */
#define BIND_MISSING  3  /* file found but can not get timestamp */
#define BIND_FOUND    4  /* file found and time stamped */

    /* update time - cleared if the there is nothing to bind */
    timestamp time;
} BINDING;

static struct hash * bindhash = 0;
static void time_enter( void *, OBJECT *, int const found,
    timestamp const * const );

static char * time_progress[] =
{
    "INIT",
    "NOENTRY",
    "SPOTTED",
    "MISSING",
    "FOUND"
};


void timestamp_clear( timestamp * const time )
{
    time->secs = 0;
}


int timestamp_cmp( timestamp const * const lhs, timestamp const * const rhs )
{
    return lhs->secs - rhs->secs;
}


void timestamp_copy( timestamp * const target, timestamp const * const source )
{
    target->secs = source->secs;
}


int timestamp_empty( timestamp const * const time )
{
    return !time->secs;
}


/*
 * timestamp_from_path() - return timestamp for a path, if present
 */

void timestamp_from_path( timestamp * const time, OBJECT * path )
{
    PROFILE_ENTER( timestamp );

    PATHNAME f1;
    PATHNAME f2;
    int found;
    BINDING * b;
    string buf[ 1 ];

    OBJECT * const normalized_path = path_as_key( path );

    string_new( buf );

    if ( !bindhash )
        bindhash = hashinit( sizeof( BINDING ), "bindings" );

    /* Quick path - is it there? */

    b = (BINDING *)hash_insert( bindhash, normalized_path, &found );
    if ( !found )
    {
        b->name = object_copy( normalized_path );  /* never freed */
        b->flags = 0;
        b->progress = BIND_INIT;
        timestamp_clear( &b->time );
    }

    if ( b->progress != BIND_INIT )
        goto afterscanning;

    b->progress = BIND_NOENTRY;

    /* Not found - have to scan for it. */
    path_parse( object_str( normalized_path ), &f1 );

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
            b->flags = 0;
            b->progress = BIND_INIT;
            timestamp_clear( &b->time );
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
            b->flags = 0;
            b->progress = BIND_INIT;
            timestamp_clear( &b->time );
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

    if ( b->progress == BIND_FOUND )
        timestamp_copy( time, &b->time );
    else
        timestamp_clear( time );

    string_free( buf );
    object_free( normalized_path );

    PROFILE_EXIT( timestamp );
}


void timestamp_init( timestamp * const time, time_t const secs )
{
    time->secs = secs;
}


void timestamp_max( timestamp * const max, timestamp const * const lhs,
    timestamp const * const rhs )
{
    if ( timestamp_cmp( lhs, rhs ) > 0 )
        timestamp_copy( max, lhs );
    else
        timestamp_copy( max, rhs );
}


char const * timestamp_str( timestamp const * const time )
{
    static char result[ 500 ];
    char format[ 500 ];
    strftime( format, sizeof( result ) / sizeof( *result ),
        "%Y-%m-%d %H:%M:%S.%%09d +0000", gmtime( &time->secs ) );
    sprintf( result, format, 0 );
    return result;
}


/*
 * time_enter() - internal worker callback for scanning archives & directories
 */

static void time_enter( void * closure, OBJECT * target, int const found,
    timestamp const * const time )
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

    timestamp_copy( &b->time, time );
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
