#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2006.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

#  Tests for the target id resolution process.
from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()

# Create the needed files
t.write("Jamroot", """
exe hello : hello.cpp ;
# This should use the 'hello' target, even if there's
# 'hello' file in current dir.
install s : hello : <location>. ;
""")
t.write("hello.cpp", """
int main()
{
    return 0;
}
""")

t.run_build_system()

t.expect_addition("bin/$toolset/debug/hello.obj")

t.touch("hello.cpp")
t.run_build_system("s")
# If 'hello' in the 's' target resolved to file in
# current dir, nothing will be rebuilt.
t.expect_touch("bin/$toolset/debug/hello.obj")

t.cleanup()
