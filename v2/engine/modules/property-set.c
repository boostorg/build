/*
 * Copyright 2013 Steven Watanabe
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include "../object.h"
#include "../lists.h"
#include "../modules.h"
#include "../rules.h"
#include "../variable.h"
#include "../native.h"
#include "../compile.h"
#include "../mem.h"

struct ps_map_entry
{
    struct ps_map_entry * next;
    LIST * key;
    OBJECT * value;
};

struct ps_map
{
    struct ps_map_entry * * table;
    size_t table_size;
    size_t num_elems;
};

static unsigned list_hash(LIST * key)
{
    unsigned int hash = 0;
    LISTITER iter = list_begin( key ), end = list_end( key );
    for ( ; iter != end; ++iter )
    {
        hash = hash * 2147059363 + object_hash( list_item( iter ) );
    }
    return hash;
}

static int list_equal( LIST * lhs, LIST * rhs )
{
    LISTITER lhs_iter, lhs_end, rhs_iter;
    if ( list_length( lhs ) != list_length( rhs ) )
    {
        return 0;
    }
    lhs_iter = list_begin( lhs );
    lhs_end = list_end( lhs );
    rhs_iter = list_begin( rhs );
    for ( ; lhs_iter != lhs_end; ++lhs_iter, ++rhs_iter )
    {
        if ( ! object_equal( list_item( lhs_iter ), list_item( rhs_iter ) ) )
        {
            return 0;
        }
    }
    return 1;
}

static void ps_map_init( struct ps_map * map )
{
    size_t i;
    map->table_size = 2;
    map->num_elems = 0;
    map->table = BJAM_MALLOC( map->table_size * sizeof( struct ps_map_entry * ) );
    for ( i = 0; i < map->table_size; ++i )
    {
        map->table[ i ] = NULL;
    }
}

static void ps_map_destroy( struct ps_map * map )
{
    size_t i;
    for ( i = 0; i < map->table_size; ++i )
    {
        struct ps_map_entry * pos;
        for ( pos = map->table[ i ]; pos; )
        {
            struct ps_map_entry * tmp = pos->next;
            BJAM_FREE( pos );
            pos = tmp;
        }
    }
    BJAM_FREE( map->table );
}

static void ps_map_rehash( struct ps_map * map )
{
    struct ps_map old = *map;
    size_t i;
    map->table = BJAM_MALLOC( map->table_size * 2 * sizeof( struct ps_map_entry * ) );
    map->table_size *= 2;
    for ( i = 0; i < map->table_size; ++i )
    {
        map->table[ i ] = NULL;
    }
    for ( i = 0; i < old.table_size; ++i )
    {
        struct ps_map_entry * pos;
        for ( pos = old.table[ i ]; pos; )
        {
            struct ps_map_entry * tmp = pos->next;

            unsigned hash_val = list_hash( pos->key );
            unsigned bucket = hash_val % map->table_size;
            pos->next = map->table[ bucket ];
            map->table[ bucket ] = pos;

            pos = tmp;
        }
    }
    BJAM_FREE( old.table );
}

static struct ps_map_entry * ps_map_insert(struct ps_map * map, LIST * key)
{
    unsigned hash_val = list_hash( key );
    unsigned bucket = hash_val % map->table_size;
    struct ps_map_entry * pos;
    for ( pos = map->table[bucket]; pos ; pos = pos->next )
    {
        if ( list_equal( pos->key, key ) )
            return pos;
    }

    if ( map->num_elems >= map->table_size )
    {
        ps_map_rehash( map );
        bucket = hash_val % map->table_size;
    }
    pos = BJAM_MALLOC( sizeof( struct ps_map_entry ) );
    pos->next = map->table[bucket];
    pos->key = key;
    pos->value = 0;
    map->table[bucket] = pos;
    ++map->num_elems;
    return pos;
}

static struct ps_map all_property_sets;

LIST * property_set_create( FRAME * frame, int flags )
{
    LIST * properties = lol_get( frame->args, 0 );
    LIST * sorted = list_sort( properties );
    LIST * unique = list_unique( sorted );
    struct ps_map_entry * pos = ps_map_insert( &all_property_sets, unique );
    list_free( sorted );
    if ( pos->value )
    {
        list_free( unique );
        return list_new( object_copy( pos->value ) );
    }
    else
    {
        OBJECT * const rulename = object_new( "new" );
        LIST * val = call_rule( rulename, frame, list_append( list_new( object_new(
            "property-set" ) ), unique ), 0 );
        /* The 'unique' variable is freed in 'call_rule'. */
        object_free( rulename );
        pos->value = list_front( val );
        pos->key = var_get( bindmodule( pos->value ), object_new( "self.raw" ) );
        return val;
    }
}


void init_property_set()
{
    char const * args[] = { "raw-properties", "*", 0 };
    declare_native_rule( "property-set", "create", args, property_set_create, 1 );
    ps_map_init( &all_property_sets );
}

void property_set_done()
{
    ps_map_destroy( &all_property_sets );
}
