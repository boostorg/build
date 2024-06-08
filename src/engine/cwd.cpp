/*
 * Copyright 2002. Vladimir Prus
 * Copyright 2005. Rene Rivera
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE.txt or copy at
 * https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#include "cwd.h"

#include "jam.h"
#include "mem.h"
#include "output.h"
#include "pathsys.h"
#include "startup.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>

/* MinGW on Windows declares PATH_MAX in limits.h */
#if defined( NT ) && !defined( __GNUC__ )
# include <direct.h>
# define PATH_MAX _MAX_PATH
#else
# include <unistd.h>
# if defined( __COMO__ )
#  include <linux/limits.h>
# endif
#endif

#ifndef PATH_MAX
# define PATH_MAX 1024
#endif


static OBJECT * cwd_;
namespace
{
    std::string cwd_s;
}


void cwd_init( void )
{
    err_puts( "Entering cwd_init()\n" );

    int buffer_size = PATH_MAX;
    char * cwd_buffer = 0;
    int error;

    assert( !cwd_ );

    err_puts( "After assert() in cwd_init\n" );

    do
    {
        char * const buffer = (char *)BJAM_MALLOC_RAW( buffer_size );

        err_puts( "After BJAM_MALLOC_RAW() in cwd_init\n" );

#ifdef OS_VMS
        /* cwd in POSIX-format */
        cwd_buffer = getcwd( buffer, buffer_size, 0 );
#else
        cwd_buffer = getcwd( buffer, buffer_size );
#endif

        err_puts( "After getcwd() in cwd_init\n" );

        error = errno;

        if ( cwd_buffer )
        {
            err_puts( "In if(cwd_buffer) in cwd_init\n" );

            /* We store the path using its canonical/long/key format. */
            OBJECT * cwd = object_new( cwd_buffer );

            err_puts( "After object_new(cwd_buffer) in cwd_init\n" );

            cwd_ = path_as_key( cwd );

            err_puts( "After path_as_key(cwd) in cwd_init\n" );

            object_free( cwd );

            err_puts( "After object_free(cwd) in cwd_init\n" );

            cwd_s = cwd_buffer;
        }

        buffer_size *= 2;
        BJAM_FREE_RAW( buffer );

        err_puts( "After BJAM_FREE_RAW(buffer) in cwd_init\n" );
    }
    while ( !cwd_ && error == ERANGE );

    err_puts( "After do..while in cwd_init\n" );

    if ( !cwd_ )
    {
        errno_puts( "can not get current working directory" );
        b2::clean_exit( EXITBAD );
    }
}


OBJECT * cwd( void )
{
    assert( cwd_ );
    return cwd_;
}


void cwd_done( void )
{
    assert( cwd_ );
    object_free( cwd_ );
    cwd_ = NULL;
}


const std::string & b2::cwd_str()
{
    return cwd_s;
}