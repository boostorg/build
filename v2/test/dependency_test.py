#!/usr/bin/python

# Copyright 2003 Dave Abrahams
# Copyright 2002, 2003, 2005, 2006 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild


def test_basic():
    t = BoostBuild.Tester(["-d3", "-d+12"], pass_d0=False)

    t.set_tree("dependency-test")

    t.run_build_system()

    # Check that main target 'c' was able to find 'x.h' from 'a's dependency
    # graph.
    t.expect_addition("bin/$toolset/debug/c.exe")

    # Check handling of first level includes.

    # Both 'a' and 'b' include "a.h" and should be updated.
    t.touch("a.h")
    t.run_build_system()

    t.expect_touch("bin/$toolset/debug/a.exe")
    t.expect_touch("bin/$toolset/debug/a.obj")
    t.expect_touch("bin/$toolset/debug/a_c.obj")
    t.expect_touch("bin/$toolset/debug/b.exe")
    t.expect_touch("bin/$toolset/debug/b.obj")
    t.expect_nothing_more()

    # Only source files using include <a.h> should be compiled.
    t.touch("src1/a.h")
    t.run_build_system()

    t.expect_touch("bin/$toolset/debug/a.exe")
    t.expect_touch("bin/$toolset/debug/a.obj")
    t.expect_touch("bin/$toolset/debug/a_c.obj")
    t.expect_nothing_more()

    # "src/a.h" includes "b.h" (in the same dir).
    t.touch("src1/b.h")
    t.run_build_system()
    t.expect_touch("bin/$toolset/debug/a.exe")
    t.expect_touch("bin/$toolset/debug/a.obj")
    t.expect_touch("bin/$toolset/debug/a_c.obj")
    t.expect_nothing_more()

    # Included by "src/b.h". We had a bug: file included using double quotes
    # (e.g. "b.h") was not scanned at all in this case.
    t.touch("src1/c.h")
    t.run_build_system()
    t.expect_touch("bin/$toolset/debug/a.exe")

    t.touch("b.h")
    t.run_build_system()
    t.expect_nothing_more()

    # Test dependency on a generated header.
    #
    # TODO: we have also to check that generated header is found correctly if
    # it is different for different subvariants. Lacking any toolset support,
    # this check will be implemented later.
    t.touch("x.foo")
    t.run_build_system()
    t.expect_touch("bin/$toolset/debug/a.obj")
    t.expect_touch("bin/$toolset/debug/a_c.obj")

    # Check that generated headers are scanned for dependencies as well.
    t.touch("src1/z.h")
    t.run_build_system()
    t.expect_touch("bin/$toolset/debug/a.obj")
    t.expect_touch("bin/$toolset/debug/a_c.obj")

    t.cleanup()


def test_scanned_includes_with_absolute_paths():
    """
      Regression test: on Windows, <includes> with absolute paths were not
    considered when scanning dependencies.

    """
    t = BoostBuild.Tester(["-d3", "-d+12"], pass_d0=False)

    t.write("jamroot.jam", """\
path-constant TOP : . ;
exe app : main.cpp : <include>$(TOP)/include ;
""");

    t.write("main.cpp", """\
#include <dir/header.h>
int main() {}
""")

    t.write("include/dir/header.h", "\n")

    t.run_build_system()
    t.expect_addition("bin/$toolset/debug/main.obj")

    t.touch("include/dir/header.h")
    t.run_build_system()
    t.expect_touch("bin/$toolset/debug/main.obj")

    t.cleanup()


test_basic()
test_scanned_includes_with_absolute_paths()
