#!/usr/bin/python

# Test usage of searched-libs: one which are found via -l
# switch to the linker/compiler. 

from BoostBuild import Tester, get_toolset
import string
import os
t = Tester()

# To start with, we have to prepate a library to link with
t.write("lib/project-root.jam", "")
t.write("lib/Jamfile", "lib test_lib : test_lib.cpp ;")
t.write("lib/test_lib.cpp", """
#ifdef _WIN32
__declspec(dllexport)
#endif
void foo() {}
""");

t.run_build_system(subdir="lib")
t.expect_addition("lib/bin/$toolset/debug/test_lib.dll")

# Auto adjusting of suffixes does not work, since we need to
# change dll to lib.
# 
if (os.name == 'nt' or os.uname()[0].lower().startswith('cygwin')) and get_toolset() != 'gcc':
    t.copy("lib/bin/$toolset/debug/test_lib.lib", "lib/test_lib.lib")
else:
    t.copy("lib/bin/$toolset/debug/test_lib.dll", "lib/libtest_lib.dll")


# Test that the simplest usage of searched library works.
t.write('project-root.jam', '')
t.write('Jamfile', """
exe main : main.cpp test_lib ;
lib test_lib : : <name>test_lib <search>lib ;
""")
t.write("main.cpp", """
void foo();
int main() { foo(); return 0; }
""")
t.run_build_system()
t.expect_addition("bin/$toolset/debug/main.exe")
t.rm("bin/$toolset/debug/main.exe")

# A regression test: <library>property referring to
# searched-lib was mishandled. As the result, we were
# putting target name to the command line!
# Note that 
#    g++ ...... <.>z
# works nicely in some cases, sending output from compiler
# to file 'z'.
# This problem shows up when searched libs are in usage
# requirements.

t.write('Jamfile', 'exe main : main.cpp d/d2//a ;')
t.write('d/d2/Jamfile', """
lib test_lib : : <name>test_lib <search>../../lib ;
lib a : a.cpp : : : <library>test_lib ;
""")
t.write('d/d2/a.cpp', """
#ifdef _WIN32
__declspec(dllexport) int force_library_creation_for_a;
#endif
""")

t.run_build_system()

# A regression test. Searched targets were not associated
# with any properties. For that reason, if the same searched
# lib is generated with two different properties, we had an
# error saying they are actualized to the same Jam target name.

t.write("project-root.jam", "")

t.write("a.cpp", "")

# The 'l' library will be built in two variants:
# 'debug' (directly requested) and 'release' (requested
# from 'a').
t.write("Jamfile", """
exe a : a.cpp l/<variant>release ;

lib l : : <name>l_d <variant>debug ;
lib l : : <name>l_r <variant>release ;
""")

t.run_build_system("-n")

# A regression test. Two virtual target with the same properties
# were created for 'l' target, which caused and error to be reported
# when actualizing targets. The final error is correct, but we should
# not create two duplicated targets. Thanks to Andre Hentz
# for finding this bug.
t.write("project-root.jam", "")

t.write("a.cpp", "")

t.write("Jamfile", """
project a : requirements <link-runtime>static ;

lib a : a.cpp l ;
lib l : : <name>l_f ;
""")

t.run_build_system("-n")


t.cleanup()
