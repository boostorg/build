#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester()


t.set_tree("project-test4")

t.run_build_system()

t.expect_addition("bin/gcc/debug/threading-single/a.obj")
t.expect_content("bin/gcc/debug/threading-single/a.obj",
"""gcc/debug/include-everything/threading-single
a.cpp
""")

t.expect_addition("bin/gcc/debug/threading-single/a.exe")
t.expect_content("bin/gcc/debug/threading-single/a.exe",
"gcc/debug/include-everything/threading-single\n" +
"bin/gcc/debug/threading-single/a.obj lib/bin/gcc/debug/optimization-on/threading-single/b.obj\n"
)

t.expect_addition("lib/bin/gcc/debug/optimization-on/threading-single/b.obj")
t.expect_content("lib/bin/gcc/debug/optimization-on/threading-single/b.obj",
"""gcc/debug/include-everything/optimization-on/threading-single
lib/b.cpp
""")

t.expect_addition("bin/gcc/debug/threading-single/main-target-b.exe/b.exe")
t.expect_content("bin/gcc/debug/threading-single/main-target-b.exe/b.exe",
"gcc/debug/define-MACROS/include-everything/threading-single\n" +
"bin/gcc/debug/threading-single/a.obj\n"
)


t.copy("lib/Jamfile2", "lib/Jamfile")

expected="""Requirements for project at 'lib' conflict with parent's.
Explanation:  link-incompatible properties <threading>single and
<threading>multi

"""
t.run_build_system(stdout=expected, status=None)

t.copy("lib/Jamfile3", "lib/Jamfile")

expected="""Cannot satisfy request to build lib/b.obj with properties  <toolset>gcc
<optimization>on <threading>single <rtti>on <debug-symbols>on <variant>debug
No viable alternative found.

"""

t.run_build_system(stdout=expected, status=None)



# We don't yet make targets depend on Jamfile, so need to start from scratch
t.set_tree("project-test4")
t.copy("Jamfile2", "Jamfile")

t.run_build_system()

t.expect_addition("bin/gcc/debug/threading-single/a_gcc.obj")
t.expect_content("bin/gcc/debug/threading-single/a_gcc.obj",
"""gcc/debug/threading-single
a_gcc.cpp
""")

t.expect_content("bin/gcc/debug/threading-single/a.exe",
"gcc/debug/threading-single\n" +
"bin/gcc/debug/threading-single/a.obj " +
"lib/bin/gcc/debug/optimization-on/threading-single/b.obj " +
"bin/gcc/debug/threading-single/a_gcc.obj\n"
)

t.cleanup()
