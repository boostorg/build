#!/usr/bin/python

# Test that we can change build directory using 
# the 'build-dir' project attribute.

from BoostBuild import Tester, exe_suffix
t = Tester()


# Test that top-level project can affect build dir
t.write("project-root.jam", "import gcc ; ")
t.write("Jamfile", """
project
    : build-dir build
    ;
    
exe a : a.cpp ;
build-project src ;    
""")
t.write("a.cpp", "int main() {}\n")

t.write("src/Jamfile", "exe b : b.cpp ; ")
t.write("b.cpp", "int main() {}\n")

t.run_build_system()

t.expect_addition(["build/bin/gcc/debug/a" + exe_suffix,
		   "build/src/bin/gcc/debug/b" + exe_suffix])
		   
# Test that project can override build dir
t.write("Jamfile", """
exe a : a.cpp ;
build-project src ;
""")		   

t.write("src/Jamfile", """
project
    : build-dir build 
    ;
exe b : b.cpp ;    
""")

t.run_build_system()
t.expect_addition(["bin/gcc/debug/a" + exe_suffix,
		   "src/build/bin/gcc/debug/b" + exe_suffix])
		   
t.cleanup()		   
