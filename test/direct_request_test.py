#!/usr/bin/python

from BoostBuild import Tester, List, dll_suffix, exe_suffix
import os
from string import strip

t = Tester()

# First check some startup
t.set_tree("direct-request-test")
t.run_build_system(extra_args="define=MACROS")

t.expect_addition("bin/gcc/debug/"
                  * (List("a.o b.o")
                     + ('b'+dll_suffix)
                     + ('a'+exe_suffix)))

t.cleanup()
