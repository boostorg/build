#!/usr/bin/python

from BoostBuild import Tester, List

t = Tester()

t.set_tree("generators-test")
t.run_build_system()

t.expect_addition("bin/gcc/debug/threading-single/" * List(   
    "a.o b.o c.h c.cpp c.o d.whl d.dlp d.cpp d.lr0 d.o x.c x.o y.x1 y.x2 " +
    "y.cpp y.o a e.marked.cpp e.positions e.target.cpp e.o e"))

t.expect_addition(["lib/bin/gcc/debug/threading-single/c.o", "lib/bin/gcc/debug/threading-single/auxilliary"])

t.cleanup()
