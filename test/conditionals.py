#!/usr/bin/python

# Test conditional properties

from BoostBuild import Tester, List
import os
from string import strip

t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("a.cpp", """
#ifdef SHARED
int main() {}
#endif
""")
t.write("Jamfile", "exe a : a.cpp : <shared>true:<define>SHARED ;")
t.run_build_system("shared=true")
t.expect_addition("bin/gcc/debug/shared-true/main-target-a/a")

t.write("Jamfile", """
project : requirements <shared>true:<define>SHARED ;
exe a : a.cpp ;
""")
t.run_build_system("shared=true")
t.expect_addition("bin/gcc/debug/shared-true/a")

t.cleanup()
