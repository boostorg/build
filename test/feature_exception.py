#!/usr/bin/env python3

# Copyright Nikita Kniazev
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild


def test_basic():
    with BoostBuild.Tester(use_test_config=False) as t:
        t.write('Jamroot.jam', 'obj test : test.cpp ;\n')
        t.write('test.cpp', 'int main() { try { throw 0; } catch (int) {} }\n')
        t.run_build_system(['exception-handling=on', 'warnings-as-errors=on'])
        t.run_build_system(['exception-handling=off', 'warnings-as-errors=on'], status=1)


def test_stdlib():
    with BoostBuild.Tester(use_test_config=False) as t:
        t.write('Jamroot.jam', 'obj test : test.cpp ;\n')
        t.write('test.cpp', '#include <iostream>\nint main() { std::cout << "Hello!"; }\n')
        t.run_build_system(['exception-handling=off', 'warnings-as-errors=on'])


test_basic()
test_stdlib()
