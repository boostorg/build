#!/usr/bin/python

# Test that default build clause actually has any effect.

from BoostBuild import Tester, exe_suffix
t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", "exe a : a.cpp : : debug release ;")
t.write("a.cpp", "int main() {}\n")

t.run_build_system()
t.expect_addition("bin/gcc/debug/a"+exe_suffix)
t.expect_addition("bin/gcc/release/a"+exe_suffix)

t.cleanup()
