#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

from BoostBuild import Tester, List
from string import find

t = Tester(pass_toolset=0)

t.write("a.cpp", """ 
""")

t.write("yfc1.jam", """ 
import feature    : extend            ;
import generators : register-standard ;

feature.extend toolset : yfc1 ;

rule init ( )
{
}

generators.register-standard yfc1.compile : CPP : OBJ : <toolset>yfc1 ;
generators.register-standard yfc1.link : OBJ : EXE : <toolset>yfc1 ;

actions compile
{
    yfc1-compile 
}

actions link
{
    yfc1-link
}
""")

t.write("yfc2.jam", """ 
import feature : extend  ;
import toolset : inherit ;

feature.extend toolset : yfc2 ;
toolset.inherit yfc2 : yfc1 ;

rule init ( )
{
}

actions link
{
    yfc2-link
} 
""")

t.write("Jamfile", """ 
exe a : a.cpp ; 
""")

t.write("project-root.jam", """
using yfc1 ;
""")

t.run_build_system("-n -d2 yfc1")
t.fail_test(find(t.stdout(), "yfc1-link") == -1)

# Make sure we don't have to explicit 'use' yfc1.
t.write("project-root.jam", """
using yfc2 ;
""")

t.run_build_system("-n -d2 yfc2")
t.fail_test(find(t.stdout(), "yfc2-link") == -1)

t.cleanup()

