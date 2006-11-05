#!/usr/bin/python

# Copyright 2003 Dave Abrahams 
# Copyright 2002, 2006 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

# Test that we can specify sources using relative names.

from BoostBuild import Tester
t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", "exe a : src/a.cpp ;")
t.write("src/a.cpp", "int main() { return 0; }\n")

t.run_build_system()

t.cleanup()
