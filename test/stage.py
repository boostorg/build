#!/usr/bin/python

# Test staging

from BoostBuild import Tester, dll_suffix
t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", """
lib a : a.cpp ;
stage dist : a a.h ;
""")
t.write("a.cpp", "")
t.write("a.h", "")

t.run_build_system()
t.expect_addition(["dist/a" + dll_suffix, "dist/a.h"])

t.cleanup()
