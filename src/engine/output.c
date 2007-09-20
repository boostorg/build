/* 
    Copyright 2007 Rene Rivera
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

#include "jam.h"
#include "output.h"
#include <stdio.h>

#define bjam_out (stdout)
#define bjam_err (stderr)

static void out_(
    const char * data,
    FILE * io
    )
{
    while ( *data )
    {
        size_t len = strcspn(data,"\r");
        fwrite(data,len,1,io);
        data += len;
        if ( *data == '\r' ) ++data;
    }
}

void out_action(
    const char * action,
    const char * target,
    const char * command,
    const char * out_data,
    const char * err_data,
    int exit_reason
    )
{
    /* print out the action+target line, if the action is quite
    the action should be null. */
    if ( action )
    {
        fprintf(bjam_out,"%s %s\n",action,target);
    }
    
    /* print out the command executed if given -d+2 */
    if ( DEBUG_EXEC )
    {
        fputs(command,bjam_out);
        fputc('\n',bjam_out);
    }
    
    /* print out the command executed to the command stream */
    if ( globs.cmdout )
    {
        fputs(command,globs.cmdout);
    }
    
    /* print out the command output, if requested */
    if (0 != out_data &&
       ( globs.pipe_action & 1 /* STDOUT_FILENO */ ||
         globs.pipe_action == 0))
    {
        out_(out_data,bjam_out);
    }
    if (0 != err_data &&
        globs.pipe_action & 2 /* STDERR_FILENO */)
    {
        out_(err_data,bjam_err);
    }
    
    fflush(bjam_out);
    fflush(bjam_err);
    fflush(globs.cmdout);
}
