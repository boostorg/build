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

t.expect_addition("bin/a.obj")
t.expect_content("bin/a.obj",
"""debug
a.cpp
""")

t.expect_addition("bin/a.exe")
t.expect_content("bin/a.exe",
"""debug
bin/a.obj
""")

t.touch("a.cpp")
t.run_build_system()
t.expect_touch(["bin/a.obj", "bin/a.exe"])
