/* (C) Copyright Rene Rivera, 2005.
** Distributed under the Boost Software License, Version 1.0.
** (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

#include <cstdio>

int main(int argc, char ** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        std::printf("arg #%u = '%s'\n",i,argv[i]);
    }
    return 1;
}
