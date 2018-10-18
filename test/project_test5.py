#!/usr/bin/python

# Copyright 2002, 2003 Dave Abrahams
# Copyright 2002, 2003, 2004, 2006 Vladimir Prus
# Copyright 2018 Rene Rivera
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild
import os

t = BoostBuild.Tester(translate_suffixes=0)

t.set_tree("project-test5")
t.run_build_system()

t.expect_addition("bin/$toolset/debug*/a.obj")
t.expect_content("bin/$toolset/debug*/a.obj", """\
$toolset/debug*
a.cpp
""")

t.expect_addition("bin/$toolset/debug*/a.exe")
t.expect_content("bin/$toolset/debug*/a.exe",
"$toolset/debug*\n" +
"bin/$toolset/debug*/a.obj lib/bin/$toolset/debug*/b.obj\n"
)

t.expect_addition("lib/bin/$toolset/debug*/b.obj")
t.expect_content("lib/bin/$toolset/debug*/b.obj", """\
$toolset/debug*
lib/b.cpp
""")

t.expect_addition("lib/bin/$toolset/debug*/m.exe")
t.expect_content("lib/bin/$toolset/debug*/m.exe", """\
$toolset/debug*
lib/bin/$toolset/debug*/b.obj
""")

t.touch("a.cpp")
t.run_build_system()
t.expect_touch(["bin/$toolset/debug*/a.obj",
                "bin/$toolset/debug*/a.exe"])

t.run_build_system(["release", "optimization=off,speed"])
t.expect_addition(["bin/$toolset/release/optimization-off*/a.exe",
                  "bin/$toolset/release/optimization-off*/a.obj",
                  "bin/$toolset/release*/a.exe",
                  "bin/$toolset/release*/a.obj"])

t.run_build_system(["--clean-all"])
t.expect_removal(["bin/$toolset/debug*/a.obj",
                 "bin/$toolset/debug*/a.exe",
                 "lib/bin/$toolset/debug*/b.obj",
                 "lib/bin/$toolset/debug*/m.exe"])

t.cleanup()
