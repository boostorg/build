#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2004. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()

t.set_tree("boostbook")
# For some reason, the messages are sent to stderr.
t.run_build_system(stderr="""Writing A.html for refentry(A)
Writing library/reference.html for section(library.reference)
Writing index.html for chapter(library)
Writing HTML.manifest
""")
t.expect_addition(["html/A.html", "html/index.html"])

t.cleanup()
