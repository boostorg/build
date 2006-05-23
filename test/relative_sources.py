#!/usr/bin/python

# Test that we can specify sources using relative names.

from BoostBuild import Tester
t = Tester()

# Test that relative path to source, 'src', is preserved.
t.write("Jamroot", "exe a : src/a.cpp ;")
t.write("src/a.cpp", "int main() { return 0; }\n")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/src/a.obj")


# Test that the relative path to source is preserved
# when using 'glob'.
t.rm("bin")
t.write("Jamroot", "exe a : [ glob src/*.cpp ] ;")
t.run_build_system()
t.expect_addition("bin/$toolset/debug/src/a.obj")


# Test that relative path with ".." is *not* added to
# target path.
t.rm(".")
t.write("Jamroot", "")
t.write("a.cpp", "int main() { return 0; }\n")
t.write("build/Jamfile", "exe a : ../a.cpp ; ")
t.run_build_system(subdir="build")
t.expect_addition("build/bin/$toolset/debug/a.obj")


t.cleanup()
