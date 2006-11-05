#!/usr/bin/python

# Copyright 2004 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  This file is template for Boost.Build tests. It creates a simple
#  project that builds one exe from one source, and checks that the exe
#  is really created.
from BoostBuild import Tester, List

# This checks that Boost.Build does not reorder <include> properties
# lexicographically.
t = Tester()

t.write("a.cpp", """ 
#include <a.h>

int main()
{
   foo();
   return 0;
}

""")

t.write("Jamfile", """ 
exe a : a.cpp : <include>d2 <include>d1 ; 
""")

t.write("project-root.jam", """ 
""")

t.write("d1/a.h", """ 
""")

t.write("d2/a.h", """ 
inline void foo() {}

""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/a.exe")

t.cleanup()
