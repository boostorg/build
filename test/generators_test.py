#!/usr/bin/python

from BoostBuild import Tester, List

t = Tester()

t.set_tree("generators-test")
t.run_build_system()

t.expect_addition("bin/gcc/debug/threading-single/" * List(   
    "a.o b.o c.h c.cpp c.o d_parser.whl d_lexer.dlp d_parser.cpp d_lexer.cpp " +
    "d_parser.lr0 x.c x.o y.x1 y.x2 " +
    "y.cpp y.o a e.marked.cpp e.positions e.target.cpp e.o e"))

t.expect_addition(["lib/bin/gcc/debug/threading-single/c.o", "lib/bin/gcc/debug/threading-single/auxilliary"])

t.cleanup()
