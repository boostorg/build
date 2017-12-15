#!/usr/bin/python

# Copyright 2008 Jurko Gospodnetic
# Copyright 2017 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Tests different aspects of Boost Builds automated testing support.

import BoostBuild

def test_run():
    t = BoostBuild.Tester(use_test_config=False)

    t.write("pass.cpp", "int main() {}\n")
    t.write("fail-compile.cpp", "#error expected to fail\n")
    t.write("fail-link.cpp", "int f();\nint main() { return f(); }\n")
    t.write("fail-run.cpp", "int main() { return 1; }\n")

    t.write("Jamroot.jam", """import testing ;
run pass.cpp ;
run fail-compile.cpp ;
run fail-link.cpp ;
run fail-run.cpp ;
""")

    t.run_build_system(status=1)
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.obj")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.exe")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.output")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.run")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.test")

    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.obj")

    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.obj")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.exe")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.output")

    t.expect_nothing_more()

    t.cleanup()

def test_run_fail():
    t = BoostBuild.Tester(use_test_config=False)

    t.write("pass.cpp", "int main() {}\n")
    t.write("fail-compile.cpp", "#error expected to fail\n")
    t.write("fail-link.cpp", "int f();\nint main() { return f(); }\n")
    t.write("fail-run.cpp", "int main() { return 1; }\n")

    t.write("Jamroot.jam", """import testing ;
run-fail pass.cpp ;
run-fail fail-compile.cpp ;
run-fail fail-link.cpp ;
run-fail fail-run.cpp ;
""")

    t.run_build_system(status=1)
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.obj")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.exe")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.output")

    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.obj")

    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.obj")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.exe")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.output")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.run")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.test")

    t.expect_nothing_more()

    t.cleanup()

def test_link():
    t = BoostBuild.Tester(use_test_config=False)

    t.write("pass.cpp", "int main() {}\n")
    t.write("fail-compile.cpp", "#error expected to fail\n")
    t.write("fail-link.cpp", "int f();\nint main() { return f(); }\n")
    t.write("fail-run.cpp", "int main() { return 1; }\n")

    t.write("Jamroot.jam", """import testing ;
link pass.cpp ;
link fail-compile.cpp ;
link fail-link.cpp ;
link fail-run.cpp ;
""")

    t.run_build_system(status=1)
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.obj")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.exe")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.test")

    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.obj")

    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.obj")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.exe")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.test")

    t.expect_nothing_more()

    t.cleanup()

def test_link_fail():
    t = BoostBuild.Tester(use_test_config=False)

    t.write("pass.cpp", "int main() {}\n")
    t.write("fail-compile.cpp", "#error expected to fail\n")
    t.write("fail-link.cpp", "int f();\nint main() { return f(); }\n")
    t.write("fail-run.cpp", "int main() { return 1; }\n")

    t.write("Jamroot.jam", """import testing ;
link-fail pass.cpp ;
link-fail fail-compile.cpp ;
link-fail fail-link.cpp ;
link-fail fail-run.cpp ;
""")

    t.run_build_system(status=1)
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.obj")

    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.obj")
    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.exe")
    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.test")

    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.obj")

    t.expect_nothing_more()

    t.cleanup()

def test_compile():
    t = BoostBuild.Tester(use_test_config=False)

    t.write("pass.cpp", "int main() {}\n")
    t.write("fail-compile.cpp", "#error expected to fail\n")
    t.write("fail-link.cpp", "int f();\nint main() { return f(); }\n")
    t.write("fail-run.cpp", "int main() { return 1; }\n")

    t.write("Jamroot.jam", """import testing ;
compile pass.cpp ;
compile fail-compile.cpp ;
compile fail-link.cpp ;
compile fail-run.cpp ;
""")

    t.run_build_system(status=1)
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.obj")
    t.expect_addition("bin/pass.test/$toolset/debug*/pass.test")

    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.obj")
    t.expect_addition("bin/fail-link.test/$toolset/debug*/fail-link.test")

    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.obj")
    t.expect_addition("bin/fail-run.test/$toolset/debug*/fail-run.test")

    t.expect_nothing_more()

    t.cleanup()

def test_compile_fail():
    t = BoostBuild.Tester(use_test_config=False)

    t.write("pass.cpp", "int main() {}\n")
    t.write("fail-compile.cpp", "#error expected to fail\n")
    t.write("fail-link.cpp", "int f();\nint main() { return f(); }\n")
    t.write("fail-run.cpp", "int main() { return 1; }\n")

    t.write("Jamroot.jam", """import testing ;
compile-fail pass.cpp ;
compile-fail fail-compile.cpp ;
compile-fail fail-link.cpp ;
compile-fail fail-run.cpp ;
""")

    t.run_build_system(status=1)
    t.expect_addition("bin/fail-compile.test/$toolset/debug*/fail-compile.obj")
    t.expect_addition("bin/fail-compile.test/$toolset/debug*/fail-compile.test")

    t.expect_nothing_more()

    t.cleanup()

################################################################################
#
# test_files_with_spaces_in_their_name()
# --------------------------------------
#
################################################################################

def test_files_with_spaces_in_their_name():
    """Regression test making sure test result files get created correctly when
    testing files with spaces in their name.
    """

    t = BoostBuild.Tester(use_test_config=False)

    t.write("valid source.cpp", "int main() {}\n");

    t.write("invalid source.cpp", "this is not valid source code");

    t.write("jamroot.jam", """
import testing ;
testing.compile "valid source.cpp" ;
testing.compile-fail "invalid source.cpp" ;
""")

    t.run_build_system(status=0)
    t.expect_addition("bin/invalid source.test/$toolset/debug*/invalid source.obj")
    t.expect_addition("bin/invalid source.test/$toolset/debug*/invalid source.test")
    t.expect_addition("bin/valid source.test/$toolset/debug*/valid source.obj")
    t.expect_addition("bin/valid source.test/$toolset/debug*/valid source.test")

    t.expect_content("bin/valid source.test/$toolset/debug*/valid source.test", \
        "passed" )
    t.expect_content( \
        "bin/invalid source.test/$toolset/debug*/invalid source.test", \
        "passed" )
    t.expect_content( \
        "bin/invalid source.test/$toolset/debug*/invalid source.obj", \
        "failed as expected" )

    t.cleanup()


################################################################################
#
# main()
# ------
#
################################################################################

test_run()
test_run_fail()
test_link()
test_link_fail()
test_compile()
test_compile_fail()
test_files_with_spaces_in_their_name()
