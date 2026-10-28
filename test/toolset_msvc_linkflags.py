#!/usr/bin/env python3
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Regression test:
# The <linkflags> passed via a using msvc : ... : options ; statement must reach the linker.

import BoostBuild
import os
import sys
from unittest.mock import patch

t = BoostBuild.Tester()

t.set_tree("toolset-mock")

# Hardcoded project-config.jam to keep it deterministic with <linkflags>
# Pass /B2_TEST_LINKFLAGS_MARKER to the linker and check that it is present in the command line as expected
t.write("project-config.jam", r"""import modules ;
import os ;

path-constant here : . ;

local PYTHON = [ os.environ PYTHON_CMD ] ;

modules.poke .ENVIRON : PROCESSOR_ARCHITEW6432 : amd64 ;

using msvc : 14.3 : :
    <compiler>\"$(PYTHON)\"\ \"$(here)/src/msvc-14.3.py\"
    <linker>\"$(PYTHON)\"\ \"$(here)/src/linkx-linkflags.py\"
    <linkflags>/B2_TEST_LINKFLAGS_MARKER
  ;
""")

# Write the mock linker script that will check for the presence of the /B2_TEST_LINKFLAGS_MARKER flag in the command line.
t.write("src/linkx-linkflags.py", r"""from MockProgram import *

if allow_properties("variant=debug", "link=shared", "threading=multi", "runtime-link=shared", "windows-api=desktop"):
    command('link', unordered('/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\lib.obj'), '/DEBUG', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', '/B2_TEST_LINKFLAGS_MARKER', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\l1.dll')), '/DLL', arg('/IMPLIB:', output_file(r'bin\msvc-14.3\debug\threading-multi\l1.implib'))))
    command('link', unordered('/NOLOGO', '/INCREMENTAL:NO', input_file(r'bin\msvc-14.3\debug\threading-multi\main.obj'), input_file(r'bin\msvc-14.3\debug\threading-multi\l1.implib'), '/DEBUG', '/MACHINE:X64', '/MANIFEST:EMBED', '/subsystem:console', '/B2_TEST_LINKFLAGS_MARKER', arg('/out:', output_file(r'bin\msvc-14.3\debug\threading-multi\test.exe'))))

main()
""")

t.set_toolset("msvc-14.3", "windows")
# Keep B2_PROPERTIES in sync with allow_properties(), scoped so it can't leak
b2_properties = {"B2_PROPERTIES": "variant=debug link=shared threading=multi runtime-link=shared windows-api=desktop"}
with patch.dict(os.environ, b2_properties):
    # Run the build and asserts that the /B2_TEST_LINKFLAGS_MARKER flag is present in the command line
    t.run_build_system(["target-os=windows", "--user-config=", "-sPYTHON_CMD=%s" % sys.executable], stderr=None)

t.expect_addition("bin/msvc-14.3/debug/threading-multi/lib.obj")
t.expect_addition("bin/msvc-14.3/debug/threading-multi/l1.implib")
t.expect_addition("bin/msvc-14.3/debug/threading-multi/l1.dll")
t.ignore_addition("bin/msvc-14.3/debug/threading-multi/*l1*.rsp")
t.expect_addition("bin/msvc-14.3/debug/threading-multi/main.obj")
t.expect_addition("bin/msvc-14.3/debug/threading-multi/test.exe")
t.ignore_addition("bin/msvc-14.3/debug/threading-multi/test.rsp")
t.expect_nothing_more()

t.cleanup()
