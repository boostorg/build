#!/usr/bin/python

# Copyright 2003, 2004 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  Test the unit_test rule
from BoostBuild import Tester, List

t = Tester()

# Create the needed files
t.write("project-root.jam", """
using testing ;
""")
t.write("Jamfile", """
lib helper : helper.cpp ;
unit-test test : test.cpp : <library>helper ;
""")
t.write("test.cpp", """
void helper();
int main()
{
    helper();
    return 0;
}
""")

t.write("helper.cpp", """
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
helper() {}
""")

t.run_build_system("link=static")
t.expect_addition("bin/$toolset/debug/link-static/test.passed")




t.cleanup()
