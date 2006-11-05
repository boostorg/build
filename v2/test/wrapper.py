#!/usr/bin/python

# Copyright 2004 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  Test that the user can define his own rule that will call builtin main
#  target rule and that this will work.

from BoostBuild import Tester, List

t = Tester()

t.write("Jamfile", """ my-test : test.cpp ;


""")

t.write("test.cpp", """ 
int main()
{
    return 0;
}

""")

t.write("project-root.jam", """ using testing ;

rule my-test ( name ? : sources + )
{
    {
        name ?= test ;
        unit-test $(name) : $(sources) ; # /site-config//cppunit /util//testMain ;
    }   
}

IMPORT $(__name__) : my-test : : my-test ;


""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/test.passed")

t.cleanup()

