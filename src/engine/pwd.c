/* Copyright Vladimir Prus 2002. Distributed under the Boost */
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



LIST*
pwd(void)
{
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) == NULL)
    {
        perror("can not get current directory");
        return L0;
    }
    else
    {
#ifdef NT
        return list_new(L0, short_path_to_long_path(buffer));
#else
        return list_new(L0, newstr(buffer));
#endif
    }
}

