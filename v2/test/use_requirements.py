#!/usr/bin/python

from BoostBuild import Tester
t = Tester()

# Test that use requirements on main target work
t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", """
    lib b : b.cpp : : : <define>FOO ;
    exe a : a.cpp b ;
""")
t.write("b.cpp", "void foo() {}")
t.write("a.cpp", """
#ifdef FOO
void foo() {}
#endif
int main() { foo(); }
""")

t.run_build_system()

t.run_build_system("--clean")

# Test that use requirements on main target work, when they are referred using
# 'dependency' features.
t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", """
    lib b : b.cpp : : : <define>FOO ;
    exe a : a.cpp : <dependency>b ;
""")
t.write("b.cpp", "void foo() {}")
t.write("a.cpp", """
#ifdef FOO
int main() {}
#endif
""")

t.run_build_system()

t.run_build_system("--clean")


# Test that use requirement on project work
t.write("Jamfile", "exe a : a.cpp lib/b ;")
t.write("lib/Jamfile", """
project :
    use-requirements <define>FOO
    ;
lib b : b.cpp ;
""")
t.write("lib/b.cpp", "void foo() {}")
t.run_build_system()

# Test that use requirements are inherited correctly

t.write("Jamfile", "exe a : a.cpp lib/1/b ;")

t.write("a.cpp", """
#if defined(FOO) && defined(ZOO)
void foo() {}
#endif
int main() { foo(); }
""")

t.write("lib/Jamfile", """
project :
    use-requirements <define>FOO
    ;
""")

t.write("lib/1/Jamfile", """
project :
    use-requirements <define>ZOO
    ;
lib b : b.cpp ;
""")
t.write("lib/1/b.cpp", "void foo() {}")

t.run_build_system()



t.cleanup()
