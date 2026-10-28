#!/usr/bin/env python3
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test for autodetecting <target-os> from the compiler's own
# `-dumpmachine` triple when target-os is not given explicitly, covering
# the gcc, clang-linux and clang-darwin toolsets.

import BoostBuild
import sys
import textwrap

MOCK_COMPILER_TEMPLATE = '''#!/usr/bin/env python3
import sys

args = sys.argv[1:]

if args == ["-dumpmachine"]:
    print("@TRIPLE@")
    sys.exit(0)

if args == ["-print-prog-name=ar"]:
    print("ar")
    sys.exit(0)

if "-c" in args and "-o" in args:
    if "-DOS_AUTODETECTED_OK" not in args:
        sys.exit(1)
    with open(args[args.index("-o") + 1], "w") as f:
        f.write("mock object\\n")
    sys.exit(0)

sys.exit(1)
'''


def test_target_os_gcc_autodetect():
    t = BoostBuild.Tester(pass_toolset=0)

    t.write("fake-compiler.py", MOCK_COMPILER_TEMPLATE.replace("@TRIPLE@", "arm-linux-androideabi"))
    t.write("project-config.jam", textwrap.dedent('''
        import os ;
        path-constant HERE : . ;
        local PYTHON = [ os.environ PYTHON_CMD ] ;
        using gcc : autodetect : $(PYTHON) $(HERE)/fake-compiler.py ;
        '''))
    t.write("Jamroot.jam", textwrap.dedent('''
        obj test : test.cpp :
        <target-os>android:<define>OS_AUTODETECTED_OK ;
        '''))
    t.write("test.cpp", "int f() { return 0; }")

    t.run_build_system(["-sPYTHON_CMD=" + sys.executable, "toolset=gcc-autodetect", "test"])
    t.cleanup()

def test_target_os_clang_linux_autodetect():
    t = BoostBuild.Tester(pass_toolset=0)

    t.write("fake-compiler.py", MOCK_COMPILER_TEMPLATE.replace("@TRIPLE@", "aarch64-linux-android21"))
    t.write("project-config.jam", textwrap.dedent('''
        import os ;
        path-constant HERE : . ;
        local PYTHON = [ os.environ PYTHON_CMD ] ;
        using clang-linux : autodetect : $(PYTHON) $(HERE)/fake-compiler.py ;
        '''))
    t.write("Jamroot.jam", textwrap.dedent('''
        obj test : test.cpp :
        <target-os>android:<define>OS_AUTODETECTED_OK ;
        '''))
    t.write("test.cpp", "int f() { return 0; }")

    t.run_build_system(["-sPYTHON_CMD=" + sys.executable, "toolset=clang-linux-autodetect", "test"])
    t.cleanup()

def test_target_os_clang_darwin_autodetect():
    t = BoostBuild.Tester(pass_toolset=0)

    t.write("fake-compiler.py", MOCK_COMPILER_TEMPLATE.replace("@TRIPLE@", "x86_64-pc-solaris2.11"))
    t.write("project-config.jam", textwrap.dedent('''
        import os ;
        path-constant HERE : . ;
        local PYTHON = [ os.environ PYTHON_CMD ] ;
        using clang-darwin : autodetect : $(PYTHON) $(HERE)/fake-compiler.py ;
        '''))
    t.write("Jamroot.jam", textwrap.dedent('''
        obj test : test.cpp :
        <target-os>solaris:<define>OS_AUTODETECTED_OK ;
        '''))
    t.write("test.cpp", "int f() { return 0; }")

    t.run_build_system(["-sPYTHON_CMD=" + sys.executable, "toolset=clang-darwin-autodetect", "test"])
    t.cleanup()


def test_target_os_clang_darwin_autodetect_linux():
    t = BoostBuild.Tester(pass_toolset=0)

    t.write("fake-compiler.py", MOCK_COMPILER_TEMPLATE.replace("@TRIPLE@", "armv7-unknown-linux-gnueabihf"))
    t.write("project-config.jam", textwrap.dedent('''
        import os ;
        path-constant HERE : . ;
        local PYTHON = [ os.environ PYTHON_CMD ] ;
        using clang-darwin : autodetect : $(PYTHON) $(HERE)/fake-compiler.py ;
        '''))
    t.write("Jamroot.jam", textwrap.dedent('''
        obj test : test.cpp :
        <target-os>linux:<define>OS_AUTODETECTED_OK ;
        '''))
    t.write("test.cpp", "int f() { return 0; }")

    t.run_build_system(["-sPYTHON_CMD=" + sys.executable, "toolset=clang-darwin-autodetect", "test"])
    t.cleanup()


test_target_os_gcc_autodetect()
test_target_os_clang_linux_autodetect()
test_target_os_clang_darwin_autodetect()
test_target_os_clang_darwin_autodetect_linux()
