#!/usr/bin/python

from BoostBuild import Tester, List
import os
from string import strip

t = Tester(translate_suffixes=0)

# First check some startup
t.set_tree("project-test3")
os.remove("project-root.jam")
t.run_build_system(status=1, stdout=
"""Failed to find the project root for directory '.'.
Did not find a project-root.jam file there or in any of its parent directories.
Please consult the documentation at 'http://www.boost.org'.
""")

t.set_tree("project-test3")
t.run_build_system()

t.expect_addition("bin/gcc/debug/a.obj")
t.expect_content("bin/gcc/debug/a.obj",
"""gcc/debug
a.cpp
""")

t.expect_addition("bin/gcc/debug/a.exe")
t.expect_content("bin/gcc/debug/a.exe",
"gcc/debug\n" +
"bin/gcc/debug/a.obj lib/bin/gcc/debug/b.obj " +
"lib2/bin/gcc/debug/c.obj lib2/bin/gcc/debug/d.obj " +
"lib2/helper/bin/gcc/debug/e.obj " +
"lib3/bin/gcc/debug/f.obj\n"
)

t.expect_addition("lib/bin/gcc/debug/b.obj")
t.expect_content("lib/bin/gcc/debug/b.obj",
"""gcc/debug
lib/b.cpp
""")

t.expect_addition("lib/bin/gcc/debug/m.exe")
t.expect_content("lib/bin/gcc/debug/m.exe",
"""gcc/debug
lib/bin/gcc/debug/b.obj lib2/bin/gcc/debug/c.obj
""")

t.expect_addition("lib2/bin/gcc/debug/c.obj")
t.expect_content("lib2/bin/gcc/debug/c.obj",
"""gcc/debug
lib2/c.cpp
""")

t.expect_addition("lib2/bin/gcc/debug/d.obj")
t.expect_content("lib2/bin/gcc/debug/d.obj",
"""gcc/debug
lib2/d.cpp
""")

t.expect_addition("lib2/bin/gcc/debug/l.exe")
t.expect_content("lib2/bin/gcc/debug/l.exe",
"""gcc/debug
lib2/bin/gcc/debug/c.obj bin/gcc/debug/a.obj
""")

t.expect_addition("lib2/helper/bin/gcc/debug/e.obj")
t.expect_content("lib2/helper/bin/gcc/debug/e.obj",
"""gcc/debug
lib2/helper/e.cpp
""")

t.expect_addition("lib3/bin/gcc/debug/f.obj")
t.expect_content("lib3/bin/gcc/debug/f.obj",
"""gcc/debug
lib3/f.cpp lib2/helper/bin/gcc/debug/e.obj
""")
                 

t.touch("a.cpp")
t.run_build_system()
t.expect_touch(["bin/gcc/debug/a.obj",
                "bin/gcc/debug/a.exe",
                "lib2/bin/gcc/debug/l.exe"])


t.run_build_system(extra_args="release optimization=off,speed")
t.expect_addition([ "bin/gcc/release/a.exe", 
		    "bin/gcc/release/a.obj", 
		    "bin/gcc/release/optimization-off/a.exe", 
		    "bin/gcc/release/optimization-off/a.obj"])

t.run_build_system(extra_args='clean')
t.expect_removal(["bin/gcc/debug/a.obj",
                 "bin/gcc/debug/a.exe",
                 "lib/bin/gcc/debug/b.obj",
                 "lib/bin/gcc/debug/m.exe",
                 "lib2/bin/gcc/debug/c.obj",
                 "lib2/bin/gcc/debug/d.obj",
                 "lib2/bin/gcc/debug/l.exe",
                 "lib3/bin/gcc/debug/f.obj",
                  ])

# Now test target ids in command line
t.set_tree("project-test3")
t.run_build_system("lib/b.obj")
t.expect_addition("lib/bin/gcc/debug/b.obj")
t.expect_nothing_more()

t.run_build_system("clean lib/b.obj")
t.expect_removal("lib/bin/gcc/debug/b.obj")
t.expect_nothing_more()

t.run_build_system("release lib2@helper/e.obj @/lib3/f.obj")
t.expect_addition("lib2/helper/bin/gcc/release/e.obj")
t.expect_addition("lib3/bin/gcc/release/f.obj")
t.expect_nothing_more()

# Test project ids in command line work as well
t.set_tree("project-test3")
t.run_build_system("@/lib2")
t.expect_addition("lib2/bin/gcc/debug/" * List("c.obj d.obj l.exe"))
t.expect_addition("bin/gcc/debug/a.obj")
t.expect_nothing_more()

t.run_build_system("lib")
t.expect_addition("lib/bin/gcc/debug/" * List("b.obj m.exe"))
t.expect_nothing_more()

t.cleanup()
