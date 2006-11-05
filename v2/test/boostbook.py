#!/usr/bin/python

# Copyright 2004, 2006 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()

t.set_tree("boostbook")
# For some reason, the messages are sent to stderr.
t.run_build_system(stderr="""Writing boost/A.html for refentry(boost.A)
Writing library/reference.html for section(library.reference)
Writing index.html for chapter(library)
Writing HTML.manifest
""")
t.expect_addition(["html/boost/A.html", "html/index.html"])

t.cleanup()
