#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2005.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

#  Basic tests for the 'notfile' rule.

from BoostBuild import Tester, List
import string
import os
import fnmatch

t = Tester()

t.write("Jamroot", """ 
import notfile ;

notfile say : "echo hi" ;

exe hello : hello.cpp ;
notfile hello_valgrind : @valgrind : hello ;

actions valgrind
{
   valgrind $(>) 
}

""")

t.write("hello.cpp", """

#include <iostream>

int main()
{
    std::cout << "Hello!\\n";
    return 1;
}

""")


t.run_build_system("-n -d+2")

t.fail_test(string.find(t.stdout(), "echo hi") == -1)

name = t.adjust_names(["bin/$toolset/debug/hello.exe"])[0]
name = apply(os.path.join, string.split(name, "/"));
c = "valgrind " + name
t.expect_output_line(c)


t.cleanup()

