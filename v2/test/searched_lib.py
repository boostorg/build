#!/usr/bin/python

# Test usage of searched-libs: one which are found via -l
# switch to the linker/compiler

from BoostBuild import Tester, exe_suffix
t = Tester()

# A regression test: <library>property referring to
# searched-lib was mishandled. As the result, we were
# putting target name to the command line!
# Note that 
#    g++ ...... <.>z
# works nicely in some cases, sending output from compiler
# to file 'z'.
# This problem shows up when searched libs are in usage
# requirements.

t.write('project-root.jam', 'import gcc ;')
t.write('Jamfile', 'exe main : main.cpp d/d2/a ;')
t.write("main.cpp", """
int main() {}
""")

t.write('d/d2/Jamfile', """
lib z : : <name>z ;
lib a : a.cpp : : : <library>z ;
""")
t.write('d/d2/a.cpp', """
""")

t.run_build_system()


t.cleanup()
