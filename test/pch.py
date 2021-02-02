#!/usr/bin/python

# Copyright 2006 Vladimir Prus.
# Copyright Nikita Kniazev 2020.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild
from time import sleep


t = BoostBuild.Tester()

t.write("jamroot.jam", """
import pch ;
project : requirements <warnings-as-errors>on ;
cpp-pch pch : pch.hpp : <toolset>msvc:<source>pch.cpp <include>. ;
cpp-pch pch-afx : pch.hpp : <define>HELLO <toolset>msvc:<source>pch.cpp <include>. ;
exe hello : hello.cpp pch : <include>. ;
exe hello-afx : hello-afx.cpp pch-afx : <define>HELLO <include>. ;
""")

pch_content = """\
#undef HELLO
class TestClass
{
public:
    TestClass( int, int ) {}
};
"""
t.write("pch.hpp", pch_content)

t.write("pch.cpp", """#include <pch.hpp>
""")

toolset = BoostBuild.get_toolset()
# Clang with posix interface always include everything in source files
if not toolset.startswith('clang') or toolset.startswith('clang-win'):
    include = '#include <pch.hpp>'
else:
    include = ''
for name in ("hello.cpp", "hello-afx.cpp"):
    t.write(name, include + """
int main() { TestClass c(1, 2); }
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/hello.exe")
t.expect_addition("bin/$toolset/debug*/hello-afx.exe")


# Now make the header unusable, replace its content with some garbage, but
# preserve the size and timestamp to fool the compiler. If everything is OK,
# B2 will not recreate PCH, and compiler will happily use pre-compiled
# header, not noticing that the real header is bad.

s = "THIS WILL NOT COMPILE. "
t.write("pch.hpp.bad", s + (len(pch_content) - len(s)) * 'x')
t.copy_timestamp("pch.hpp", "pch.hpp.bad")
t.copy_preserving_timestamp("pch.hpp.bad", "pch.hpp")

t.rm("bin/$toolset/debug*/hello.obj")
t.rm("bin/$toolset/debug*/hello-afx.obj")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/hello.obj")
t.expect_addition("bin/$toolset/debug*/hello-afx.obj")

t.cleanup()
