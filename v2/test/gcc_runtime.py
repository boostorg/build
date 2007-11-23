#!/usr/bin/python

# Copyright 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#  Tests that on gcc, we correctly report problem when static runtime
#  is requested when building DLL.
from BoostBuild import Tester, List
import string

t = Tester()

# Create the needed files
t.write("project-root.jam", "")
t.write("Jamfile", """
lib hello : hello.cpp ;
""")
t.write("hello.cpp", """
int main()
{
    return 0;
}
""")

t.run_build_system("runtime-link=static", status=1)
t.fail_test(string.find(t.stdout(),
                        "On gcc, DLL can't be build with '<runtime-link>static'") == -1)

t.run_build_system("link=static runtime-link=static")
t.expect_addition("bin/$toolset/debug/link-static/runtime-link-static/hello.lib")

t.cleanup()
