#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester()
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
