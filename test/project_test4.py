#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester()


t.set_tree("project-test4")

t.run_build_system()

t.expect_addition("bin/gcc/debug/a.obj")
t.expect_content("bin/gcc/debug/a.obj",
"""gcc/debug/include-everything
a.cpp
""")

t.expect_addition("bin/gcc/debug/a.exe")
t.expect_content("bin/gcc/debug/a.exe",
"gcc/debug/include-everything\n" +
"bin/gcc/debug/a.obj lib/bin/gcc/debug/optimization-on/b.obj\n"
)

t.expect_addition("lib/bin/gcc/debug/optimization-on/b.obj")
t.expect_content("lib/bin/gcc/debug/optimization-on/b.obj",
"""gcc/debug/include-everything/optimization-on
lib/b.cpp
""")

t.expect_addition("bin/gcc/debug/main-target-b.exe/b.exe")
t.expect_content("bin/gcc/debug/main-target-b.exe/b.exe",
"gcc/debug/define-MACROS/include-everything\n" +
"bin/gcc/debug/a.obj\n"
)


t.copy("lib/Jamfile2", "lib/Jamfile")

expected="""error: Requirements for project at 'lib' conflict with parent's.
Explanation:  link-incompatible properties <threading>single and <threading>multi
"""
t.run_build_system("--no-error-backtrace", stdout=expected, status=None)

t.copy("lib/Jamfile3", "lib/Jamfile")

expected="""warning: skipped build of lib/b.obj with properties <toolset>gcc
<shared>false <optimization>on <threading>single <rtti>on <debug-symbols>on
<variant>debug
don't know how to make <.>lib/b.obj/<optimization>on
...skipped <./gcc/debug>a.exe for lack of <.>lib/b.obj/<optimization>on...
"""

t.run_build_system(stdout=expected, status=None)

# Check that project can be skipped as well
t.copy("Jamfile4", "Jamfile")

expected="""warning: skipping build of project at lib2
"""
t.run_build_system("rtti=on", stdout=expected, status=None)

t.copy("lib2/Jamfile2", "lib2/Jamfile")

expected="""warning: skipping build of project /mylib at lib2
"""

t.run_build_system("rtti=on", stdout=expected, status=None)

# We don't yet make targets depend on Jamfile, so need to start from scratch
# The following test is disabled, because of problems related to
# issue 634319

#t.set_tree("project-test4")
#t.copy("Jamfile2", "Jamfile")

#t.run_build_system()

#t.expect_addition("bin/gcc/debug/a_gcc.obj")
#t.expect_content("bin/gcc/debug/a_gcc.obj",
#"""gcc/debug
#a_gcc.cpp
#""")

#t.expect_content("bin/gcc/debug/a.exe",
#"gcc/debug\n" +
#"bin/gcc/debug/a.obj " +
#"lib/bin/gcc/debug/optimization-on/b.obj " +
#"bin/gcc/debug/a_gcc.obj\n"
#)

# Test that if we specified composite property in target reference,
# everything works OK.

t.copy("lib/Jamfile1", "lib/Jamfile")
t.copy("Jamfile5", "Jamfile")

t.run_build_system()

t.expect_addition("lib/bin/gcc/release/b.obj")

t.expect_content("bin/gcc/debug/a.exe",
"gcc/debug/include-everything\n" +
"bin/gcc/debug/a.obj lib/bin/gcc/release/b.obj\n"
)


t.cleanup()
