/*
 * Copyright 1993, 1995 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  (C) Copyright David Abrahams 2001. Permission to copy, use,
 *  modify, sell and distribute this software is granted provided this
 *  copyright notice appears in all copies. This software is provided
 *  "as is" without express or implied warranty, and with no claim as
 *  to its suitability for any purpose.
 */

/*
 * command.c - maintain lists of commands
 */

# include "jam.h"

# include "lists.h"
# include "parse.h"
# include "variable.h"
# include "rules.h"

# include "command.h"
# include <limits.h>

/*
 * cmd_new() - return a new CMD or 0 if too many args
 */

CMD *
cmd_new(
	RULE	*rule,
	LIST	*targets,
	LIST	*sources,
	LIST	*shell )
{
    CMD *cmd = (CMD *)malloc( sizeof( CMD ) );
    /* lift line-length limitation entirely when JAMSHELL is just "%" */
    int expand_line = ( shell && !strcmp(shell->string,"%") && !list_next(shell) );
    int max_line = MAXLINE;
    int allocated = -1;

    cmd->rule = rule;
    cmd->shell = shell;
    cmd->next = 0;

    lol_init( &cmd->args );
    lol_add( &cmd->args, targets );
    lol_add( &cmd->args, sources );
    cmd->buf = 0;

    do
    {
        free(cmd->buf); /* free any buffer from previous iteration */
        
        cmd->buf = (char*)malloc(max_line + 1);
        
        if (cmd->buf == 0)
            break;
        
        allocated = var_string( rule->actions->command, cmd->buf, max_line, &cmd->args );
        
        max_line = max_line * 2;
    }
    while( expand_line && allocated < 0 && max_line < INT_MAX / 2 );
    
    /* Bail if the result won't fit in MAXLINE */
    /* We don't free targets/sources/shell if bailing. */
    if( allocated < 0 )
    {
        cmd_free( cmd );
        cmd = 0;
    }

    return cmd;
}

/*
 * cmd_free() - free a CMD
 */

void
cmd_free( CMD *cmd )
{
	lol_free( &cmd->args );
	list_free( cmd->shell );
    free( cmd->buf );
	free( (char *)cmd );
}
