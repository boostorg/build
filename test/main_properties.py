#!/usr/bin/python

# This tests a bug where files were places to a directory corresponding to a main
# target that was using its main target.

from BoostBuild import Tester
t = Tester()

# In order to correctly link this app, 'b.cpp', created by 'make'
# rule, should be compiled.
t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", """
lib b : b.cpp : <define>BAR ;
exe a : a.cpp b ;
""")
t.write("a.cpp", """
void foo();
int main() { foo(); }
""")
t.write("b.cpp", "void foo() {}")

t.run_build_system()
t.expect_addition("bin/gcc/debug/main-target-b/b.o")


t.cleanup()
