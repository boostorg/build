#!/usr/bin/python

from BoostBuild import Tester, List

t = Tester()

t.set_tree("dependency-test")
t.run_build_system()
# Check that main target 'c' was able to find 'x.h' from
# 'a's dependency graph
t.expect_addition("bin/gcc/debug/main-target-c/c")
# Check that main target 'e' was able to find 'y.h'
t.expect_addition("bin/gcc/debug/main-target-e/e")

# Check handling of first level includes.

# Both 'a' and 'b' include "a.h" and should be updated
t.touch("a.h")
t.run_build_system()

t.expect_touch("bin/gcc/debug/a")
t.expect_touch("bin/gcc/debug/a.o")
t.expect_touch("bin/gcc/debug/b")
t.expect_touch("bin/gcc/debug/b.o")
t.expect_touch("bin/gcc/debug/main-target-c/c")
t.expect_nothing_more()

# Only 'a' include <a.h> and should be updated
t.touch("src1/a.h")
t.run_build_system()

t.expect_touch("bin/gcc/debug/a")
t.expect_touch("bin/gcc/debug/a.o")
t.expect_touch("bin/gcc/debug/main-target-c/c")
t.expect_nothing_more()

# "src/a.h" includes "b.h" (in the same dir)
t.touch("src1/b.h")
t.run_build_system()
t.expect_touch("bin/gcc/debug/a")
t.expect_touch("bin/gcc/debug/a.o")
t.expect_touch("bin/gcc/debug/main-target-c/c")
t.expect_nothing_more()

t.touch("b.h")
t.run_build_system()
t.expect_nothing_more()

# Test dependency on generated header.
# TODO: we have also to check that generated header is found correctly
# if it is different for different subvariants. Lacking any toolset
# support, this check will be implemented later.
t.touch("x.foo")
t.run_build_system()
t.expect_touch("bin/gcc/debug/a.o")

t.cleanup()
