#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  Regression test: when staging V2 used to change suffixes on targets
#  corresponding to real files.

from BoostBuild import Tester, List

t = Tester()

t.write("Jamfile", """ 
import type : register ;
register A : a1 a2 a3 ;

stage a : a.a3 ; 
""")

t.write("project-root.jam", "")
t.write("a.a3", "")

t.run_build_system()
t.expect_addition("a/a.a3");

t.cleanup()

