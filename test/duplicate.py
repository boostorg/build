#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2004. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  This test tries to stage the same file to the same location by *two*
#  different stage rules, in two different projects. This is not exactly
#  good thing to do, but still, V2 should handle this. We had two bugs:
#  - since the file is referred from two projects, we created to different
#    virtual targets
#  - we also failed to figure out that the two target corresponding to the
#    copied files (created in two projects) are actually equivivalent.

from BoostBuild import Tester, List


t = Tester()

t.write("a.cpp", """ 
""")

t.write("Jamfile", """ 
build-project a ;
build-project b ; 
""")

t.write("project-root.jam", """ 
""")

t.write("a/Jamfile", """ 
stage bin : ../a.cpp : <location>../dist ; 
""")

t.write("b/Jamfile", """ 
stage bin : ../a.cpp : <location>../dist ; 
""")

t.run_build_system()
t.expect_addition("dist/a.cpp")


t.cleanup()
