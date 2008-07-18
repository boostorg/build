#!/usr/bin/python
#
# Copyright (c) 2008 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt) or copy at
# http://www.boost.org/LICENSE_1_0.txt)

from BoostBuild import Tester

tester = Tester()

# test without giving the project an explicit id.
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

# this time, do give the project an id.
tester.write("Jamroot", """
    lib test : test.cpp ;
    project test_project : requirements <library>test ;
    build-project a ;
""")

tester.run_build_system()

tester.expect_addition("bin/$toolset/debug/test.obj")
tester.expect_addition("a/bin/$toolset/debug/test1.exe")

tester.cleanup()
