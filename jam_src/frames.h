#ifndef FRAMES_DWA20011021_H
# define FRAMES_DWA20011021_H

# include "lists.h"
# include "modules.h"

typedef struct frame FRAME;

struct frame
{
    FRAME* prev;
    LOL args[1];
    module* module;
};

void frame_init( FRAME* ); /* implemented in compile.c */
void frame_free( FRAME* ); /* implemented in compile.c */

#endif // FRAMES_DWA20011021_H
