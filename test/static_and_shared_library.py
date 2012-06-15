#!/usr/bin/python

# Copyright 2002, 2003 Dave Abrahams
# Copyright 2002, 2003, 2005 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild
import os.path
import shutil

t = BoostBuild.Tester()
t.write("jamroot.jam", "")
t.write("lib/c.cpp", "int bar() { return 0; }\n")
t.write("lib/jamfile.jam", """\
static-lib auxilliary1 : c.cpp ;
lib auxilliary2 : c.cpp ;
""")

def reset():
    shutil.rmtree(os.path.join(t.workdir, "lib", "bin"))

t.run_build_system(subdir='lib')
t.expect_addition("lib/bin/$toolset/debug/" * BoostBuild.List("c.obj "
    "auxilliary1.lib auxilliary2.dll"))
t.expect_nothing_more()

reset()
t.run_build_system(subdir='lib', extra_args="link=shared")
t.expect_addition("lib/bin/$toolset/debug/" * BoostBuild.List("c.obj "
    "auxilliary1.lib auxilliary2.dll"))
t.expect_nothing_more()

reset()
t.run_build_system(subdir='lib', extra_args="link=static")
t.expect_addition("lib/bin/$toolset/debug/link-static/" * BoostBuild.List(
    "c.obj auxilliary1.lib auxilliary2.lib"))
t.expect_nothing_more()

t.cleanup()
