#!/usr/bin/python
#
# Copyright (c) 2008 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt) or copy at
# http://www.boost.org/LICENSE_1_0.txt)

from BoostBuild import Tester

tester = Tester()


###############################################################################
#
# Test without giving the project an explicit id.
#
###############################################################################

tester.write("Jamroot", """
    lib test : test.cpp ;
    project : requirements <library>test ;
    build-project a ;
""")

tester.write("test.cpp", """
    #ifdef _WIN32
        __declspec(dllexport)
    #endif
    void foo() {}
""")

tester.write("a/test1.cpp", """
    int main() {}
""")

tester.write("a/Jamfile", """
    exe test1 : test1.cpp ;
""")

tester.run_build_system()

tester.expect_addition("bin/$toolset/debug/test.obj")
tester.expect_addition("a/bin/$toolset/debug/test1.exe")

tester.rm("bin")
tester.rm("a/bin")


###############################################################################
#
# Run the same test from the "a" directory.
#
###############################################################################

tester.run_build_system(subdir="a")

tester.expect_addition("bin/$toolset/debug/test.obj")
tester.expect_addition("a/bin/$toolset/debug/test1.exe")

tester.rm("bin")
tester.rm("a/bin")


###############################################################################
#
# This time, do give the project an id.
#
###############################################################################

tester.write("Jamroot", """
    lib test : test.cpp ;
    project test_project : requirements <library>test ;
    build-project a ;
""")

tester.run_build_system()

tester.expect_addition("bin/$toolset/debug/test.obj")
tester.expect_addition("a/bin/$toolset/debug/test1.exe")

tester.rm("bin")
tester.rm("a/bin")


###############################################################################
#
# Now, give the project an id in its attributes.
#
###############################################################################

tester.write("Jamroot", """
    lib test : test.cpp ;
    project : id test_project : requirements <library>test ;
    build-project a ;
""")

tester.run_build_system()

tester.expect_addition("bin/$toolset/debug/test.obj")
tester.expect_addition("a/bin/$toolset/debug/test1.exe")

tester.rm("bin")
tester.rm("a/bin")


###############################################################################
#
# Give the project an id in both ways at once.
#
###############################################################################

tester.write("Jamroot", """
    lib test : test.cpp ;
    project test_project1 : id test_project : requirements <library>test ;
    build-project a ;
""")

tester.run_build_system()

tester.expect_addition("bin/$toolset/debug/test.obj")
tester.expect_addition("a/bin/$toolset/debug/test1.exe")

tester.rm("bin")
tester.rm("a/bin")


###############################################################################
#
# Test an absolute path in native format.
#
###############################################################################

tester.write("Jamroot", """
    import path ;
    path-constant here : . ;
    current-location =
        [ path.native [ path.root [ path.make $(here) ] [ path.pwd ] ] ] ;
    project test : requirements <source>$(current-location)/a/test1.cpp ;
    exe test : test.cpp ;
""")

tester.run_build_system()
tester.expect_addition("bin/$toolset/debug/test.exe")

tester.rm("bin")
tester.rm("a/bin")


###############################################################################
#
# Test an absolute path in canonical format.
#
###############################################################################

tester.write("Jamroot", """
    import path ;
    path-constant here : . ;
    current-location = [ path.root [ path.make $(here) ] [ path.pwd ] ] ;
    project test : requirements <source>$(current-location)/a/test1.cpp ;
    exe test : test.cpp ;
""")

tester.run_build_system()
tester.expect_addition("bin/$toolset/debug/test.exe")

tester.rm("bin")
tester.rm("a/bin")


###############################################################################
#
# Test dependency properties (e.g. <source>) whose targets are specified using a
# relative path.
#
###############################################################################

# Use Jamroot rather than Jamfile to avoid inheriting the <source> from the
# parent as that would would make test3 a source of itself.
tester.write("b/Jamroot", """
    obj test3 : test3.cpp ;
""")

tester.write("b/test3.cpp", """
    void bar() {}
""")

tester.write("Jamroot", """
    project test : requirements <source>b//test3 ;
    build-project a ;
""")

tester.write("a/Jamfile", """
    exe test : test1.cpp ;
""")

tester.write("a/test1.cpp", """
    void bar();
    int main() { bar(); }
""")

tester.run_build_system()
tester.expect_addition("b/bin/$toolset/debug/test3.obj")
tester.expect_addition("a/bin/$toolset/debug/test.exe")

tester.rm("bin")
tester.rm("a")
tester.rm("Jamroot")
tester.rm("test.cpp")


###############################################################################
#
# Test that source-location is respected
#
###############################################################################

tester.write("build/Jamroot", """
    project : requirements <source>test.cpp : source-location ../src ;
""")

tester.write("src/test.cpp", """
    int main() {}
""")

tester.write("build/a/Jamfile", """
    project : source-location ../../a_src ;
    exe test : test1.cpp ;
""")

tester.write("a_src/test1.cpp", """
""")

tester.run_build_system(subdir="build/a")
tester.expect_addition("build/a/bin/$toolset/debug/test.exe")

tester.cleanup()
