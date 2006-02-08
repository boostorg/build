#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2006.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

#  Test that it's possible to add a suffix to a main target name to
#  disambiguate that main target from another, and that this does not
#  affect the names of the generated targets.
from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()

# Create the needed files
t.write("Jamroot", """
exe hello.exe : hello.obj ;
obj hello.obj : hello.cpp : <variant>debug ;
obj hello.obj2 : hello.cpp : <variant>release ;
""")
t.write("hello.cpp", """
int main()
{
    return 0;
}

""")

t.run_build_system()

t.expect_addition("bin/$toolset/debug/hello.exe")
t.expect_addition("bin/$toolset/debug/hello.obj")
t.expect_addition("bin/$toolset/release/hello.obj")


t.cleanup()
