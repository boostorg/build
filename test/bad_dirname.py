#!/usr/bin/python

# Copyright 2003 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  Regression test: when directory of project root contained regex metacharacters,
#  Boost.Build failed to work. Bug reported by Michael Stevens

from BoostBuild import Tester, List

t = Tester()

t.write("bad[abc]dirname/Jamfile", """ 
""")

t.write("bad[abc]dirname/project-root.jam", """ 
""")

t.run_build_system(subdir="bad[abc]dirname")

t.cleanup()

