/*  
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef FRAMES_DWA20011021_H
# define FRAMES_DWA20011021_H

# include "lists.h"
# include "modules.h"

typedef struct _PARSE PARSE;
typedef struct frame FRAME;

struct frame
{
    FRAME* prev;
    LOL args[1];
    module_t* module;
    PARSE* procedure;
    char*  rulename;
};

void frame_init( FRAME* ); /* implemented in compile.c */
void frame_free( FRAME* ); /* implemented in compile.c */

#endif

