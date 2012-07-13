/* Copyright Vladimir Prus 2003. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#include "../native.h"
#include "../object.h"
#include "../timestamp.h"


LIST * path_exists( FRAME * frame, int flags )
{
    time_t time;
    timestamp_from_target( list_front( lol_get( frame->args, 0 ) ), &time );
    return time ? list_new( object_new( "true" ) ) : L0;
}


void init_path()
{
    char const * args[] = { "location", 0 };
    declare_native_rule( "path", "exists", args, path_exists, 1 );
}
