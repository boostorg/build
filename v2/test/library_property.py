#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2004. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  Test that the <library> property has no effect on "obj" targets.
#  Previously, it affected all targets, so
#
#     project : requirements <library>foo ;
#     exe a : a.cpp helper ;
#     obj helper : helper.cpp : <optimization>off ;
#
#  caused 'foo' to be built with with and without optimization.
from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()

t.write("Jamfile", """ 
project
   : requirements <library>lib//x
   ;
exe a : a.cpp foo ;
obj foo : foo.cpp : <variant>release ; 
""")

t.write("a.cpp", """
void aux();
int main() { aux(); }
""")

t.write("foo.cpp", """
void gee();
void aux() { gee(); }
""")

t.write("project-root.jam", """ 
""")

t.write("lib/x.cpp", """
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
gee() {}
""")

t.write("lib/Jamfile", """ 
lib x : x.cpp ;
""")

t.write("lib/project-root.jam", """ 
""")


t.run_build_system()
t.expect_addition("bin/$toolset/debug/a.exe")
t.expect_nothing("lib/bin/$toolset/release/x.obj")
t.cleanup()


