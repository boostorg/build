#!/usr/bin/python

from BoostBuild import Tester, List
import os

t = Tester()

t.set_tree("generators-test")
t.run_build_system()

t.expect_addition(
    "bin/gcc/debug/"
    * (
    List(   
       "a.o b.o c.h c.cpp c.o d_parser.whl d_lexer.dlp d_parser.cpp d_lexer.cpp "
        + "d_parser.lr0 x.c x.o y.x1 y.x2 "
        + "y.cpp y.o e.marked.cpp e.positions e.target.cpp e.o "
        + "a.exe e.exe"))
    )

t.expect_addition(["lib/bin/gcc/debug/c.o",
                   "lib/bin/gcc/debug/auxilliary.lib",
                   ])


t.run_build_system(subdir='lib')

t.expect_addition(["lib/bin/gcc/debug/auxilliary2.dll"])

t.run_build_system(subdir='lib', extra_args="link=static")

t.expect_addition(["lib/bin/gcc/debug/link-static/auxilliary2.lib"])

t.cleanup()
