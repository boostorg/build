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

typedef struct timing_info
{
    /* double elapsed; */  /* We don't know how to get this number on Unix */
    double system;
    double user;
} timing_info;

void execcmd(
	char *string,
	void (*func)( void *closure, int status, timing_info* ),
	void *closure,
	LIST *shell );

int execwait();

# define EXEC_CMD_OK	0
# define EXEC_CMD_FAIL	1
# define EXEC_CMD_INTR	2
