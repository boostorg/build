#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  Test that C files are compiled by C compiler
from BoostBuild import Tester, List

t = Tester()

t.write("project-root.jam", "")
t.write("Jamfile", """
project ;

exe hello : hello.cpp a.c ;
""")
t.write("hello.cpp", """
extern "C" int foo();
int main() { return foo(); }
""")
t.write("a.c", """
// This won't compile unless in C mode
int foo()
{
    int new = 0;
    new = (new+1)*7;
    return new;
}
""")
t.run_build_system()
t.expect_addition("bin/$toolset/debug/hello.exe")

t.cleanup()
