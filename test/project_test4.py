#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester()


t.set_tree("project-test4")

t.run_build_system()

t.expect_addition("bin/gcc/debug/shared-false/threading-single/a.obj")
t.expect_content("bin/gcc/debug/shared-false/threading-single/a.obj",
"""gcc/debug/include-everything/shared-false/threading-single
a.cpp
""")

t.expect_addition("bin/gcc/debug/shared-false/threading-single/a.exe")
t.expect_content("bin/gcc/debug/shared-false/threading-single/a.exe",
"gcc/debug/include-everything/shared-false/threading-single\n" +
"bin/gcc/debug/shared-false/threading-single/a.obj lib/bin/gcc/debug/optimization-on/shared-false/threading-single/b.obj\n"
)

t.expect_addition("lib/bin/gcc/debug/optimization-on/shared-false/threading-single/b.obj")
t.expect_content("lib/bin/gcc/debug/optimization-on/shared-false/threading-single/b.obj",
"""gcc/debug/include-everything/optimization-on/shared-false/threading-single
lib/b.cpp
""")

t.expect_addition("bin/gcc/debug/shared-false/threading-single/main-target-b.exe/b.exe")
t.expect_content("bin/gcc/debug/shared-false/threading-single/main-target-b.exe/b.exe",
"gcc/debug/define-MACROS/include-everything/shared-false/threading-single\n" +
"bin/gcc/debug/shared-false/threading-single/a.obj\n"
)


t.copy("lib/Jamfile2", "lib/Jamfile")

expected="""Requirements for project at 'lib' conflict with parent's.
Explanation:  link-incompatible properties <threading>single and
<threading>multi

"""
t.run_build_system(stdout=expected, status=None)

t.copy("lib/Jamfile3", "lib/Jamfile")

expected="""warning: skipped build of lib/b.obj with properties <toolset>gcc
<shared>false <optimization>on <threading>single <rtti>on <debug-symbols>on
<variant>debug
don't know how to make <.>lib/b.obj/<optimization>on
...skipped <./gcc/debug/shared-false/threading-single>a.exe for lack of <.>lib/b.obj/<optimization>on...
"""

t.run_build_system(stdout=expected, status=None)

# Check that project can be skipped as well
t.copy("Jamfile4", "Jamfile")

expected="""warning: skipping build of project at lib2
"""
t.run_build_system(stdout=expected, status=None)

t.copy("lib2/Jamfile2", "lib2/Jamfile")

expected="""warning: skipping build of project /mylib at lib2
"""

t.run_build_system(stdout=expected, status=None)

# We don't yet make targets depend on Jamfile, so need to start from scratch
t.set_tree("project-test4")
t.copy("Jamfile2", "Jamfile")

t.run_build_system()

t.expect_addition("bin/gcc/debug/shared-false/threading-single/a_gcc.obj")
t.expect_content("bin/gcc/debug/shared-false/threading-single/a_gcc.obj",
"""gcc/debug/shared-false/threading-single
a_gcc.cpp
""")

t.expect_content("bin/gcc/debug/shared-false/threading-single/a.exe",
"gcc/debug/shared-false/threading-single\n" +
"bin/gcc/debug/shared-false/threading-single/a.obj " +
"lib/bin/gcc/debug/optimization-on/shared-false/threading-single/b.obj " +
"bin/gcc/debug/shared-false/threading-single/a_gcc.obj\n"
)

t.cleanup()
