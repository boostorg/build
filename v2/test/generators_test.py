#!/usr/bin/python

# Copyright 2003 Dave Abrahams
# Copyright 2002, 2003, 2005 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild

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

t.cleanup()
