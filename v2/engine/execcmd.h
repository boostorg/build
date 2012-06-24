/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * execcmd.h - execute a shell script.
 *
 * Defines the interface to be implemented in platform specific implementation
 * modules as well as different shared utility functions prepared in the
 * execcmd.c module.
 */

#ifndef EXECCMD_H
#define EXECCMD_H

#include "lists.h"
#include "strings.h"
#include <time.h>

typedef struct timing_info
{
    double system;
    double user;
    time_t start;
    time_t end;
} timing_info;

typedef void (* ExecCmdCallback)
(
    void * closure,
    int status,
    timing_info *,
    char const * invoked_command,
    char const * command_output
) ;

void exec_cmd
(
    string const * command,
    ExecCmdCallback func,
    void * closure,
    LIST * shell,
    char const * action,
    char const * target
);

int exec_wait();

#define EXEC_CMD_OK    0
#define EXEC_CMD_FAIL  1
#define EXEC_CMD_INTR  2


/******************************************************************************
 *                                                                            *
 * Utility functions defined in the execcmd.c module.                         *
 *                                                                            *
 ******************************************************************************/

/* Constructs a list of command-line elements using the format specified by the
 * given shell list.
 */
void argv_from_shell( char const * * argv, LIST * shell, char const * command,
    int const slot );

/* Interrupt routine bumping the internal interrupt counter. Needs to be
 * registered by platform specific exec*.c modules.
 */
void onintr( int disp );

/* Returns whether an interrupt has been detected so far. */
int interrupted( void );

/* Checks whether the given shell list is actually a request to execute raw
 * commands without an external shell.
 */
int is_raw_command_request( LIST * shell );

#endif
