#!/usr/bin/python

# Copyright 2003 Dave Abrahams
# Copyright 2002, 2006 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test that we can specify sources using relative names.

import BoostBuild

t = BoostBuild.Tester()

t.write("jamroot.jam", "import gcc ;")
t.write("jamfile.jam", "exe a : src/a.cpp ;")
t.write("src/a.cpp", "int main() {}\n")

t.run_build_system()

t.cleanup()
