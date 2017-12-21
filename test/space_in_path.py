#!/usr/bin/python

# Copyright 2012 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test that paths containing spaces are handled correctly by actions.

import BoostBuild
import os

t = BoostBuild.Tester(use_test_config=False)

t.write("has space/jamroot.jam", """\
import testing ;
unit-test test : test.cpp ;
actions write-file
{
    @(STDOUT:E=okay) >"$(<)"
}
make test.txt : : @write-file ;
""")
t.write("has space/test.cpp", "int main() {}\n")

tmpdir = t.workpath("has space")
os.environ["TMP"] = tmpdir; # Windows
os.environ["TMPDIR"] = tmpdir; # *nix

t.run_build_system(["has space"])

t.expect_addition("has space/bin/$toolset/debug*/test.txt")
t.expect_addition("has space/bin/$toolset/debug*/test.passed")

t.cleanup()
