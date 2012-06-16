#!/usr/bin/python

# Copyright 2003 Dave Abrahams
# Copyright 2002, 2003, 2005 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild
import re

def match_count_is(lines, pattern, expected):
    count = 0
    for x in lines:
        if re.search(pattern, x):
            count += 1
        if count > expected:
            return False
    return count == expected


t = BoostBuild.Tester()
t.set_tree("generators-test")

t.run_build_system("-d1")
t.expect_addition("bin/$toolset/debug/" * BoostBuild.List("a.my_exe a.my_obj "
    "b.my_obj c.tui_h c.cpp c.my_obj d_parser.whl d_lexer.dlp d_parser.cpp "
    " d_lexer.cpp d_lexer.my_obj d_parser.lr0 d_parser.h d_parser.my_obj "
    "d_parser_symbols.h x.c x.my_obj y.x1 y.x2 y.cpp y.my_obj e.marked_cpp "
    "e.positions e.target_cpp e.my_obj e.my_exe f.my_exe obj_1.my_obj "
    "obj_2.my_obj"))
t.expect_addition("lib/bin/$toolset/debug/" * BoostBuild.List("c.my_obj "
    "auxilliary.my_lib"))
t.expect_nothing_more()

t.expect_content_line("bin/$toolset/debug/obj_1.my_obj", "Sources: 'z.cpp'")
t.expect_content_line("bin/$toolset/debug/obj_2.my_obj", "Sources: 'z.cpp'")
t.expect_content_line("bin/$toolset/debug/a.my_obj", "Sources: 'a.cpp'")

lines = t.stdout().splitlines()
source_lines = [x for x in lines if re.match("^     Sources: '", x)]
if not match_count_is(source_lines, "'z.cpp'", 2):
    BoostBuild.annotation("failure", "z.cpp must be compiled exactly twice.")
    t.fail_test(1)
if not match_count_is(source_lines, "'a.cpp'", 1):
    BoostBuild.annotation("failure", "a.cpp must be compiled exactly once.")
    t.fail_test(1)

t.cleanup()
