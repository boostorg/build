#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2005.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

from BoostBuild import Tester, List
import string

t = Tester()

t.write("sub/a.cpp", """ 
int main() { return 0; }

""")

t.write("Jamroot", """ 
using some_tool ;
""")

t.write("some_tool.jam", """ 
import project ;
project.initialize $(__name__) ;

rule init ( )
{
}

""")

t.write("sub/Jamfile", """ 
exe a : a.cpp ; 
""")



t.run_build_system(subdir="sub")

t.expect_addition("sub/bin/$toolset/debug/a.exe")

t.cleanup()

