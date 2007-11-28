/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*
 * execcmd.h - execute a shell script
 *
 * 05/04/94 (seiwald) - async multiprocess interface
 */

#ifndef EXECCMD_H
#define EXECCMD_H

#include <time.h>

typedef struct timing_info
{
    double system;
    double user;
    time_t start;
    time_t end;
} timing_info;

void execcmd(
    char *string,
    void (*func)( void *closure, int status, timing_info*, char *, char * ),
    void *closure,
    LIST *shell,
        char *action,
        char *target);

int execwait();

# define EXEC_CMD_OK    0
# define EXEC_CMD_FAIL  1
# define EXEC_CMD_INTR  2

#endif
