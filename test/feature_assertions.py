#!/usr/bin/env python

# Copyright 2020 Nikita Kniazev
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

# Tests the force-include feature

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("Jamroot.jam", """
import testing ;
run test.c : <variant>debug                  : : : debug_assert ;
run test.c : <variant>debug <assertions>off  : : : debug_noassert ;
run test.c : <variant>release                : : : release_noassert ;
run test.c : <variant>release <assertions>on : : : release_assert ;
""")

t.write("test.c", """
#include <stdio.h>
int main(int argc, const char* argv[])
{
#ifdef NDEBUG
    puts("+NDEBUG");
#else
    puts("-NDEBUG");
#endif
}
""")

t.run_build_system()
t.expect_content("bin/*/debug_assert.output", "-NDEBUG\n\nEXIT STATUS: 0")
t.expect_content("bin/*/debug_noassert.output", "+NDEBUG\n\nEXIT STATUS: 0")
t.expect_content("bin/*/release_noassert.output", "+NDEBUG\n\nEXIT STATUS: 0")
t.expect_content("bin/*/release_assert.output", "-NDEBUG\n\nEXIT STATUS: 0")

t.cleanup()
