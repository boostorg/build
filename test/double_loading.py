#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  Regression test for double loading of the same Jamfile.
from BoostBuild import Tester, List
import string

t = Tester()

t.write("Jamfile", """ 
build-project subdir ; 
""")

t.write("project-root.jam", """ 
""")

t.write("subdir/Jamfile", """ 
ECHO "Loaded subdir" ; 
""")

t.run_build_system(subdir="subdir")
t.fail_test(string.count(t.stdout(), "Loaded subdir") != 1)

t.cleanup()

