#!/usr/bin/python

from BoostBuild import Tester, List

t = Tester()

t.set_tree("dependency-test")
t.run_build_system()
# Do not bother checking for created files now.



# Check handling of first level includes.

# Both 'a' and 'b' include "a.h" and should be updated
t.touch("a.h")
t.run_build_system()

t.expect_touch("bin/gcc/debug/shared-false/threading-single/a")
t.expect_touch("bin/gcc/debug/shared-false/threading-single/a.o")
t.expect_touch("bin/gcc/debug/shared-false/threading-single/b")
t.expect_touch("bin/gcc/debug/shared-false/threading-single/b.o")
t.expect_nothing_more()

# Only 'a' include <a.h> and should be updated
t.touch("src1/a.h")
t.run_build_system()

t.expect_touch("bin/gcc/debug/shared-false/threading-single/a")
t.expect_touch("bin/gcc/debug/shared-false/threading-single/a.o")
t.expect_nothing_more()

# "src/a.h" includes "b.h" (in the same dir)
t.touch("src1/b.h")
t.run_build_system()
t.expect_touch("bin/gcc/debug/shared-false/threading-single/a")
t.expect_touch("bin/gcc/debug/shared-false/threading-single/a.o")
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
t.expect_touch("bin/gcc/debug/shared-false/threading-single/a.o")

t.cleanup()
