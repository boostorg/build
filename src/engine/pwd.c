#include "jam.h"
#include "lists.h"
#include "newstr.h"
#include "pathsys.h"

#ifdef NT
#include <direct.h>
#define PATH_MAX _MAX_PATH
#else
#include <unistd.h>
#endif

#include <limits.h>

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

