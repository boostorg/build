#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

from BoostBuild import Tester, List

t = Tester()

# Regression tests: standalone project were not able to refer to targets
# declared in themselfs!

t.write("a.cpp", """ 
int main()
{
    return 0;
}

""")

t.write("project-root.jam", """ 
import standalone ; 
""")

t.write("standalone.jam", """ 
import project ;

project.initialize $(__name__) ;
project standalone ;

local pwd = [ PWD ] ;

alias x : $(pwd)/../a.cpp ;
alias runtime : x ;

""")

t.write("sub/Jamfile", """ 
stage bin : /standalone//runtime ; 
""")

t.run_build_system(subdir="sub")
t.expect_addition("sub/bin/a.cpp")
t.cleanup()

