#!/usr/bin/python

# Copyright 2003 Dave Abrahams
# Copyright 2002, 2003, 2005 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild

t = BoostBuild.Tester()
t.set_tree("generators-test")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/" * BoostBuild.List("a.exe a.obj "
    "b.obj c.h c.cpp c.obj d_parser.whl d_lexer.dlp d_parser.cpp d_lexer.cpp "
    "d_parser.lr0 d_parser.h d_parser_symbols.h x.c x.obj y.x1 y.x2 y.cpp "
    "y.obj e.marked_cpp e.positions e.target_cpp e.obj"))
t.expect_addition("lib/bin/$toolset/debug/" * BoostBuild.List("c.obj "
    "auxilliary.lib"))

t.cleanup()
