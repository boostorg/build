#!/usr/bin/python

from BoostBuild import Tester, List
import os

t = Tester()

t.set_tree("generators-test")
t.run_build_system()

t.expect_addition(
    "bin/$toolset/debug/"
    * (
    List(   
       "a.obj b.obj c.h c.cpp c.obj d_parser.whl d_lexer.dlp d_parser.cpp d_lexer.cpp "
        + "d_parser.lr0 d_parser.h d_parser_symbols.h x.c x.obj y.x1 y.x2 "
        + "y.cpp y.obj e.marked.cpp e.positions e.target.cpp e.obj "))
    )
ok = 0
# Target names differ on toolsets.
#print t.unexpected_difference.added_files
#print t.adjust_names("bin/$toolset/debug/main-target-a/a.exe")
if t.adjust_names("bin/$toolset/debug/main-target-a/a.exe")[0] in t.unexpected_difference.added_files:
    ok = 1
elif t.adjust_names("bin/$toolset/debug/a.exe")[0] in t.unexpected_difference.added_files:
    ok = 1

t.fail_test(ok != 1)    

t.expect_addition(["lib/bin/$toolset/debug/c.obj",
                   "lib/bin/$toolset/debug/auxilliary.lib",
                   ])


t.run_build_system(subdir='lib')

t.expect_addition(["lib/bin/$toolset/debug/auxilliary2.dll"])

t.run_build_system(subdir='lib', extra_args="link=static")

t.expect_addition(["lib/bin/$toolset/debug/link-static/auxilliary2.lib"])

t.cleanup()
