/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * timestamp.h - get the timestamp of a file or archive member
 */

#ifndef TIMESTAMP_H_SW_2011_11_18
#define TIMESTAMP_H_SW_2011_11_18

#include "object.h"

#include <time.h>

typedef struct timestamp
{
    time_t secs;
} timestamp;

void timestamp_clear( timestamp * const time );
int timestamp_cmp( timestamp const * const lhs, timestamp const * const rhs );
void timestamp_copy( timestamp * const target, timestamp const * const source );
int timestamp_empty( timestamp const * const time );
void timestamp_from_path( timestamp * const time, OBJECT * path );
void timestamp_init( timestamp * const time, time_t const secs );
void timestamp_max( timestamp * const max, timestamp const * const lhs,
    timestamp const * const rhs );
char const * timestamp_str( timestamp const * const time );

void timestamp_done();

#endif
