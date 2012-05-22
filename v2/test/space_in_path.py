#!/usr/bin/python

# Copyright 2012 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test that paths containing spaces are handled correctly by actions.

import BoostBuild

class unsplit(object):
    def __init__(self, value):
        self.value = value
    def split(self, ignored):
        return self.value

t = BoostBuild.Tester()

t.write("has space/Jamroot.jam", """
import testing ;
unit-test test : test.cpp ;
""")

t.write("has space/test.cpp", """
int main() {}
""")

t.run_build_system(unsplit(["has space"]))

t.cleanup()
