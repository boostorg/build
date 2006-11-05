#!/usr/bin/python

# Copyright 2003 Douglas Gregor 
# Copyright 2005 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

from BoostBuild import Tester, List

t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", """
import print ;
print.output foo ;
print.text \\\"Something\\\" ;
DEPENDS all : foo ;
ALWAYS foo ;
""")

t.run_build_system()
t.expect_content("foo", """\"Something\"""")

t.write("Jamfile", """
import print ;
print.output foo ;
print.text \\\n\\\"Somethingelse\\\" ;
DEPENDS all : foo ;
ALWAYS foo ;
""")

t.run_build_system()
t.expect_content("foo", """\"Something\"
\"Somethingelse\"""")

t.write("Jamfile", """
import print ;
print.output foo ;
print.text \\\"Different\\\" : true ;
DEPENDS all : foo ;
ALWAYS foo ;
""")

t.run_build_system()
t.expect_content("foo", """\"Different\"""")

t.cleanup()
