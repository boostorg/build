#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2006.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

#  Tests that we can build a file (not target), by it's name
from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()

# Create the needed files
t.write("Jamroot", """
exe hello : hello.cpp ;
build-project sub ;
""")
t.write("hello.cpp", """
int main()
{
    return 0;
}
""")
t.write("sub/Jamfile", """
exe hello : hello.cpp ;
""")
t.write("sub/hello.cpp", """
int main()
{
    return 0;
}
""")


t.run_build_system(t.adjust_suffix("hello.obj"))

t.expect_addition("bin/$toolset/debug/hello.obj")
t.expect_addition("sub/bin/$toolset/debug/hello.obj")
t.expect_nothing_more()

# Remove temporary directories
t.cleanup()
