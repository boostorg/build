#!/usr/bin/python

# Copyright 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

# Tests that on gcc, we correctly report a problem when static runtime is
# requested for building a shared library.

import BoostBuild

t = BoostBuild.Tester()
t.write("jamroot.jam", "exe hello : hello.cpp ; outline mangled : hello ;")
t.write("hello.cpp", "extern \"C\" int i; int main() { i = 42; return 0;}\n") 
t.write("script.ld", "i = 42")

# Also testing linker-scripts. If that thing works, the extern "C" int i can be resolved, elsewise not.

t.run_build_system(["runtime-link=static"])
t.expect_output_lines("warning: On gcc, DLLs can not be built with "
    "'<runtime-link>static'.")
t.expect_nothing_more()

t.run_build_system(["link=static", "runtime-link=static"])
binFolder = "bin/$toolset/debug/link-static/runtime-link-static"
t.expect_addition("%s/hello.obj" % binFolder)
t.expect_addition("%s/hello.lib" % binFolder)
t.expect_nothing_more()

t.cleanup()
