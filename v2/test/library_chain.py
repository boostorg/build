#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  Test that a chain of libraries work ok, not matter if we use static or
#  shared linking.
from BoostBuild import Tester, List
import string

t = Tester()

t.write("Jamfile", """
# Stage the binary, so that it will be relinked
# without hardcode-dll-paths. That will chech that
# we pass correct -rpath-link, even if not passing
# -rpath.
stage dist : main ;
exe main : main.cpp b ; 
""")

t.write("main.cpp", """ 
void foo();

int main() { foo(); return 0; }

""")

t.write("project-root.jam", """ 
""")

t.write("a/a.cpp", """
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
gee() {}
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
geek() {} 
""")

t.write("a/Jamfile", """ 
lib a : a.cpp ; 
""")

t.write("b/b.cpp", """ 
void geek();

void 
#if defined(_WIN32)
__declspec(dllexport)
#endif
foo() { geek(); }

""")

t.write("b/Jamfile", """ 
lib b : b.cpp ../a//a ; 
""")

t.run_build_system(stderr=None)
t.expect_addition("bin/$toolset/debug/main.exe")
t.rm(["bin", "a/bin", "b/bin"])

t.run_build_system("link=static")
t.expect_addition("bin/$toolset/debug/link-static/main.exe")
t.rm(["bin", "a/bin", "b/bin"])

# Check that <library> works for static linking.

t.write("b/Jamfile", """ 
lib b : b.cpp : <library>../a//a ; 
""")
t.run_build_system("link=static")
t.expect_addition("bin/$toolset/debug/link-static/main.exe")

t.rm(["bin", "a/bin", "b/bin"])
t.write("b/Jamfile", """ 
lib b : b.cpp ../a//a/<link>shared : <link>static ; 
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/main.exe")
t.rm(["bin", "a/bin", "b/bin"])

# Test that putting library in sources of a searched library
# works.
t.write("Jamfile", """
exe main : main.cpp png ;
lib png : z : <name>png ;
lib z : : <name>zzz ;
""")
t.run_build_system("-a -n -d+2")
t.fail_test(string.find(t.stdout(), "zzz") == -1)



t.cleanup()
