/*
 *  Copyright 2012 Jurko Gospodnetic
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * filent.h - Windows specific file routines
 */

#ifndef FILENT_JG20120714_H
#define FILENT_JG20120714_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <time.h>

double filetime_to_seconds( FILETIME const ft );
time_t filetime_to_timestamp( FILETIME const ft );

#endif
