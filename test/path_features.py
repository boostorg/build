#!/usr/bin/python

from BoostBuild import Tester
t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", "lib a : a.cpp : <include>. ;")
t.write("a.cpp", """
#include <a.h>
void
# ifdef _WIN32
__declspec(dllexport)
# endif 
foo() {}
""")
t.write("a.h", "")

t.write("d/Jamfile", "exe b : b.cpp ../a ; ")
t.write("d/b.cpp", """
    void foo();
    int main() { foo(); }
""")

t.run_build_system(subdir="d")

# Now test the path features with condition work as well
t.write("Jamfile", "lib a : a.cpp : <variant>debug:<include>.  ;")
t.rm("bin")
t.run_build_system(subdir="d")

# Test path features with condtion in usage requirements
t.write("Jamfile", "lib a : a.cpp : <include>. : : <variant>debug:<include>. ;")
t.write("d/b.cpp", """
#include <a.h>
void foo();
int main() { foo(); }
""")
t.rm("d/bin")
t.run_build_system(subdir="d")


t.cleanup()
