#!/usr/bin/python

# Copyright 2003 Dave Abrahams 
# Copyright 2003, 2006 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

# Test the 'make' rule

from BoostBuild import Tester
from string import find

t = Tester(pass_toolset=1)

t.write("Jamroot", """

import feature ;
feature.feature test_feature : : free ;

import toolset ;
toolset.flags creator STRING : <test_feature> ;

actions creator
{
    echo $(STRING) > $(<)
}

make foo.bar : : creator : <test_feature>12345678 ;
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/foo.bar")
t.fail_test(find(t.read("bin/$toolset/debug/foo.bar"), "12345678") == -1)

# Regression test. Make sure that if main target requested two times,
# and build request differ only in incidental properties, the main target
# if created only once. The bug was discovered by Kirill Lapshin.

t.write("Jamroot", """ 
# Make sure that incidental property does not
# cause second creation of 'hello1.cpp'.
exe a : dir//hello1.cpp ;
exe b : dir//hello1.cpp/<hardcode-dll-paths>true ; 
""")

t.write("dir/Jamfile", """ 
import common ;
make hello1.cpp : hello.cpp : common.copy ;

""")

t.write("dir/hello.cpp", """
int main()
{
    return 1;
}
""")
# Show only names of the actions.
t.run_build_system("-d1 -n")
t.fail_test(t.stdout().count("copy") != 1)

t.cleanup()
