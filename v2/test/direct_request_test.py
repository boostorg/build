#!/usr/bin/python

from BoostBuild import Tester, List
import os
from string import strip

t = Tester()

# First check some startup
t.set_tree("direct-request-test")
t.run_build_system(extra_args="define=MACROS")

t.expect_addition("bin/gcc/debug/" * List("a.o b.o b.a a"))

t.cleanup()
