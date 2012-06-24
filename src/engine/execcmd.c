/*
 * Copyright 1993, 1995 Christopher Seiwald.
 * Copyright 2007 Noel Belcourt.
 *
 *   Utility functions shared between different exec*.c platform specific
 * implementation modules.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

#include "jam.h"
#include "execcmd.h"


/* Internal interrupt counter. */
static int intr;


/* Returns whether an interrupt has been detected so far. */

int interrupted( void )
{
    return intr != 0;
}


/* Internal interrupt handler. */

void onintr( int disp )
{
    ++intr;
    printf( "...interrupted\n" );
}
