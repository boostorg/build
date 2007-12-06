/* Copyright Vladimir Prus 2002, Rene Rivera 2005. Distributed under the Boost */
/* Software License, Version 1.0. (See accompanying */
/* file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#include "jam.h"
#include "lists.h"
#include "newstr.h"
#include "pathsys.h"

#include <limits.h>

/* MinGW on windows declares PATH_MAX in limits.h */
#if defined(NT) && ! defined(__GNUC__)
#include <direct.h>
#define PATH_MAX _MAX_PATH
#else
#include <limits.h>
#include <unistd.h>
#if defined(__COMO__)
     #include <linux/limits.h>
#endif
#endif

/* The current directory can't change in bjam, so optimize this to cache
** the result.
*/
static char pwd_buffer[PATH_MAX];
static char * pwd_result = NULL;


LIST*
pwd(void)
{
    if (!pwd_result)
    {
        if (getcwd(pwd_buffer, sizeof(pwd_buffer)) == NULL)
        {
            perror("can not get current directory");
            return L0;
        }
        else
        {
#ifdef NT
            pwd_result = short_path_to_long_path(pwd_buffer);
#else
            pwd_result = newstr(pwd_buffer);
#endif
        }
    }
    return list_new(L0, pwd_result);
}

