#!/usr/bin/python

# Test staging

from BoostBuild import Tester, dll_suffix
t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", """
lib a : a.cpp ;
stage dist : a a.h auxilliary/1 ;
""")
t.write("a.cpp", "")
t.write("a.h", "")
t.write("auxilliary/1", "")

t.run_build_system()
t.expect_addition(["dist/a" + dll_suffix, "dist/a.h", "dist/1"])

t.cleanup()
