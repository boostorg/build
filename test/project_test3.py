#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester()
t.set_tree("project-test3")


t.run_build_system()

t.expect_addition("bin/a.obj")

expected = """debug
a.cpp
"""

if t.read("bin/a.obj") != expected:
    print '***expected:'
    print expected
    print '***got this instead:'
    print t.read("bin/a.obj")
    t.fail_test(1)

t.expect_addition("bin/a.exe")
t.fail_test(t.read("bin/a.exe") !=
"""debug
bin/a.obj
""")

t.touch("a.cpp")
t.run_build_system()
t.expect_touch(["bin/a.obj", "bin/a.exe"])
