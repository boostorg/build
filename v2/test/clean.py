#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2006.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

from BoostBuild import Tester, List
import string

t = Tester()

t.write("a.cpp", """ 
int main() {}

""")

t.write("Jamroot", """ 
exe a : a.cpp sub1//sub1 sub2//sub2 sub3//sub3 ; 
""")

t.write("sub1/Jamfile", """ 
lib sub1 : sub1.cpp ../sub2//sub2 ; 
""")

t.write("sub1/sub1.cpp", """
#ifdef _WIN32
__declspec(dllexport)
#endif
void sub1() {}

""")

t.write("sub2/Jamfile", """ 
lib sub2 : sub2.cpp ; 
""")

t.write("sub2/sub2.cpp", """
#ifdef _WIN32
__declspec(dllexport)
#endif
void sub2() {}

""")

t.write("sub3/Jamroot", """ 
lib sub3 : sub3.cpp ; 
""")

t.write("sub3/sub3.cpp", """
#ifdef _WIN32
__declspec(dllexport)
#endif
void sub3() {}

""")


# The 'clean' should not remove files under separate Jamroot.
t.run_build_system()
t.run_build_system("--clean")
t.expect_removal("bin/$toolset/debug/a.obj")
t.expect_removal("sub1/bin/$toolset/debug/sub1.obj")
t.expect_removal("sub2/bin/$toolset/debug/sub2.obj")
t.expect_nothing("sub3/bin/$toolset/debug/sub3.obj")

# The 'clean-all' removes everything it can reach.
t.run_build_system()
t.run_build_system("--clean")
t.expect_removal("bin/$toolset/debug/a.obj")
t.expect_removal("sub1/bin/$toolset/debug/sub1.obj")
t.expect_removal("sub2/bin/$toolset/debug/sub2.obj")
t.expect_nothing("sub3/bin/$toolset/debug/sub3.obj")

# The 'clean' together with project target removes
# only under that probject
t.run_build_system()
t.run_build_system("sub1 --clean")
t.expect_nothing("bin/$toolset/debug/a.obj")
t.expect_removal("sub1/bin/$toolset/debug/sub1.obj")
t.expect_nothing("sub2/bin/$toolset/debug/sub2.obj")
t.expect_nothing("sub3/bin/$toolset/debug/sub3.obj")

# And clean-all removes everything.
t.run_build_system()
t.run_build_system("sub1 --clean-all")
t.expect_nothing("bin/$toolset/debug/a.obj")
t.expect_removal("sub1/bin/$toolset/debug/sub1.obj")
t.expect_removal("sub2/bin/$toolset/debug/sub2.obj")
t.expect_nothing("sub3/bin/$toolset/debug/sub3.obj")






t.cleanup()

