#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester()

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

t.expect_addition("bin/gcc/debug/threading-single/a.obj")
t.expect_content("bin/gcc/debug/threading-single/a.obj",
"""gcc/debug/threading-single
a.cpp
""")

t.expect_addition("bin/gcc/debug/threading-single/a.exe")
t.expect_content("bin/gcc/debug/threading-single/a.exe",
"gcc/debug/threading-single\n" +
"bin/gcc/debug/threading-single/a.obj lib/bin/gcc/debug/threading-single/b.obj " +
"lib2/bin/gcc/debug/threading-single/c.obj lib2/bin/gcc/debug/threading-single/d.obj " +
"lib2/helper/bin/gcc/debug/threading-single/e.obj " +
"lib3/bin/gcc/debug/threading-single/f.obj\n"
)

t.expect_addition("lib/bin/gcc/debug/threading-single/b.obj")
t.expect_content("lib/bin/gcc/debug/threading-single/b.obj",
"""gcc/debug/threading-single
lib/b.cpp
""")

t.expect_addition("lib/bin/gcc/debug/threading-single/m.exe")
t.expect_content("lib/bin/gcc/debug/threading-single/m.exe",
"""gcc/debug/threading-single
lib/bin/gcc/debug/threading-single/b.obj lib2/bin/gcc/debug/threading-single/c.obj
""")

t.expect_addition("lib2/bin/gcc/debug/threading-single/c.obj")
t.expect_content("lib2/bin/gcc/debug/threading-single/c.obj",
"""gcc/debug/threading-single
lib2/c.cpp
""")

t.expect_addition("lib2/bin/gcc/debug/threading-single/d.obj")
t.expect_content("lib2/bin/gcc/debug/threading-single/d.obj",
"""gcc/debug/threading-single
lib2/d.cpp
""")

t.expect_addition("lib2/bin/gcc/debug/threading-single/l.exe")
t.expect_content("lib2/bin/gcc/debug/threading-single/l.exe",
"""gcc/debug/threading-single
lib2/bin/gcc/debug/threading-single/c.obj bin/gcc/debug/threading-single/a.obj
""")

t.expect_addition("lib2/helper/bin/gcc/debug/threading-single/e.obj")
t.expect_content("lib2/helper/bin/gcc/debug/threading-single/e.obj",
"""gcc/debug/threading-single
lib2/helper/e.cpp
""")

t.expect_addition("lib3/bin/gcc/debug/threading-single/f.obj")
t.expect_content("lib3/bin/gcc/debug/threading-single/f.obj",
"""gcc/debug/threading-single
lib3/f.cpp lib2/helper/bin/gcc/debug/threading-single/e.obj
""")
                 

t.touch("a.cpp")
t.run_build_system()
t.expect_touch(["bin/gcc/debug/threading-single/a.obj",
                "bin/gcc/debug/threading-single/a.exe",
                "lib2/bin/gcc/debug/threading-single/l.exe"])


t.run_build_system(extra_args="release optimization=off,on")
t.expect_addition([ "bin/gcc/release/threading-single/a.exe", 
		    "bin/gcc/release/threading-single/a.obj", 
		    "bin/gcc/release/optimization-off/threading-single/a.exe", 
		    "bin/gcc/release/optimization-off/threading-single/a.obj"])

t.run_build_system(extra_args='clean')
t.expect_removal(["bin/gcc/debug/threading-single/a.obj",
                 "bin/gcc/debug/threading-single/a.exe",
                 "lib/bin/gcc/debug/threading-single/b.obj",
                 "lib/bin/gcc/debug/threading-single/m.exe",
                 "lib2/bin/gcc/debug/threading-single/c.obj",
                 "lib2/bin/gcc/debug/threading-single/d.obj",
                 "lib2/bin/gcc/debug/threading-single/l.exe",
                 "lib3/bin/gcc/debug/threading-single/f.obj",
                  ])

t.cleanup()
