#!/usr/bin/python

# Test that sources with absolute names are handled OK.

from BoostBuild import Tester
t = Tester()

t.write("project-root.jam", """
path-constant TOP : . ;
""")
t.write("Jamfile", """
local pwd = [ PWD ] ;
ECHO $(pwd) XXXXX ;
exe hello : $(pwd)/hello.cpp $(TOP)/empty.cpp ;
""")
t.write("hello.cpp", "int main() { return 0; }\n")
t.write("empty.cpp", "\n")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/hello.exe")

t.cleanup()
