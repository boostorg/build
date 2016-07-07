#!/usr/bin/python

# Copyright 2016. Arvid Norberg
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

# Test correct "-c" option handling.

import BoostBuild
import MockToolset

t = BoostBuild.Tester(arguments=['toolset=mock', '--ignore-site-config',
    '--user-config='], pass_toolset=0)

MockToolset.create(t)
MockToolset.set_expected(t, '''
action('-c -x c++ hello.cpp -o bin/mock/debug/hello.o')
action('bin/mock/debug/hello.o -o bin/mock/debug/hello')
''')

t.write("hello.cpp", 'int main() { return 0; }\n')
t.write("Jamroot.jam", "exe test : hello.cpp ;\n")

t.run_build_system(['-ca'])
t.ignore("bin")

t.expect_addition("compile_commands.json")
with open('compile_commands.json') as f:
	l = f.readlines()
	assert len(l) == 3
	assert l[0] == '[\n'
	assert l[1].startswith('{ "directory": "')
	assert l[1].endswith('mock.py -c -x c++  \\"hello.cpp\\" -o \\"bin/mock/debug/hello.o\\"", "file": "hello.cpp" },\n')
	assert l[2] == ']\n'

t.expect_nothing_more()
t.cleanup()

