#!/usr/bin/python

# Test conditional properties

from BoostBuild import Tester, List, exe_suffix
import os
from string import strip

t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("a.cpp", """
#ifdef STATIC
int main() {}
#endif
""")
t.write("Jamfile", "exe a : a.cpp : <link>static:<define>STATIC ;")
t.run_build_system("link=static")
t.expect_addition("bin/gcc/debug/link-static/main-target-a/a" + exe_suffix)

t.write("Jamfile", """
project : requirements <link>static:<define>STATIC ;
exe a : a.cpp ;
""")
t.run_build_system("link=static")
t.expect_addition("bin/gcc/debug/link-static/a" + exe_suffix)

t.cleanup()
