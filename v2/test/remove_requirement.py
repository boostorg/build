#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2006.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

from BoostBuild import Tester, List
import string

t = Tester()

t.write("Jamroot", """ 
project : requirements <threading>multi <variant>debug:<link>static ;

build-project sub ;
build-project sub2 ;
build-project sub3 ;
build-project sub4 ; 
""")

t.write("sub/Jamfile", """ 
exe hello : hello.cpp : -<threading>multi ; 
""")

t.write("sub/hello.cpp", """ 
int main()
{
    return 0;
}

""")

t.write("sub2/Jamfile", """ 
project : requirements -<threading>multi ;
exe hello : hello.cpp ; 
""")

t.write("sub2/hello.cpp", """ 
int main()
{
    return 0;
}

""")

t.write("sub3/hello.cpp", """ 
int main()
{
    return 0;
}

""")

t.write("sub3/Jamfile", """ 
exe hello : hello.cpp : -<variant>debug:<link>static ; 
""")

t.write("sub4/hello.cpp", """ 
int main()
{
    return 0;
}

""")

t.write("sub4/Jamfile", """ 
project : requirements -<variant>debug:<link>static ;
exe hello : hello.cpp ; 
""")



t.run_build_system()

t.expect_addition("sub/bin/$toolset/debug/link-static/hello.exe")
t.expect_addition("sub2/bin/$toolset/debug/link-static/hello.exe")
t.expect_addition("sub3/bin/$toolset/debug/threading-multi/hello.exe")
t.expect_addition("sub4/bin/$toolset/debug/threading-multi/hello.exe")

t.rm(".")

# Now test that path requirements can be removed as well.
t.write("Jamroot", """ 
build-project sub ;

""")

t.write("sub/Jamfile", """ 
project : requirements <include>broken ;

exe hello : hello.cpp : -<include>broken ; 
""")

t.write("sub/hello.cpp", """ 
#include "math.h"

int main()
{
    return 0;
}

""")

t.write("sub/broken/math.h", """ 
Broken 
""")


t.run_build_system()

t.expect_addition("sub/bin/$toolset/debug/hello.exe")

t.cleanup()

