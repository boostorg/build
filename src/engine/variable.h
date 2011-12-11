/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * variable.h - handle jam multi-element variables
 */

#ifndef VARIABLE_SW20111119_H
#define VARIABLE_SW20111119_H

struct hash;

void    var_defines( char* const *e, int preprocess );
LIST *  var_get( OBJECT * symbol );
void    var_set( OBJECT * symbol, LIST * value, int flag );
LIST *  var_swap( OBJECT * symbol, LIST *value );
void    var_done();
void    var_hash_swap( struct hash * * );

/*
 * Defines for var_set().
 */

# define VAR_SET    0   /* override previous value */
# define VAR_APPEND 1   /* append to previous value */
# define VAR_DEFAULT    2   /* set only if no previous value */

#endif
