#!/usr/bin/python

# Test that we can specify sources using relative names.

from BoostBuild import Tester
t = Tester()

t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", "exe a : src/a.cpp ;")
t.write("src/a.cpp", "int main() {}")

t.run_build_system()

t.cleanup()
