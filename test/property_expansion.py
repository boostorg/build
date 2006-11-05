#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

from BoostBuild import Tester, List

# Test that free property inside 
t = Tester()

t.write("project-root.jam", "")
t.write("Jamfile", """
project ;

variant debug-AA : debug : <define>AA ;

alias all : hello ;
exe hello : hello.cpp ;
explicit hello ;
""")
t.write("hello.cpp", """
#ifdef AA
int main()
{
    return 0;
}
#endif
""")

t.run_build_system("debug-AA")


t.cleanup()
