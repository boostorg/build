#!/usr/bin/python

# Copyright 2012. Jurko Gospodnetic
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test correct "-p" option handling.

import BoostBuild

t = BoostBuild.Tester("-d1", pass_d0=False, pass_toolset=False)

t.write("file.jam", """\
prefix = "echo \"" ;
suffix = "\"" ;
if $(NT)
{
    prefix = "(echo " ;
    suffix = ")" ;
}
actions go
{
    $(prefix)stdout$(suffix)
    $(prefix)stderr$(suffix) 1>&2
}
ECHO {{{ $(XXX) }}} ;
ALWAYS all ;
go all ;
""")

t.run_build_system("-ffile.jam -sXXX=1", stderr="")
t.expect_output_line("{{{ 1 }}}")
t.expect_output_line("stdout")
t.expect_output_line("stderr")
t.expect_nothing_more()

t.run_build_system("-ffile.jam -sXXX=2 -p0", stderr="")
t.expect_output_line("{{{ 2 }}}")
t.expect_output_line("stdout")
t.expect_output_line("stderr")
t.expect_nothing_more()

t.run_build_system("-ffile.jam -sXXX=3 -p1", stderr="")
t.expect_output_line("{{{ 3 }}}")
t.expect_output_line("stdout")
t.expect_output_line("stderr*", False)
t.expect_nothing_more()

t.run_build_system("-ffile.jam -sXXX=4 -p2", stderr="stderr\n")
t.expect_output_line("{{{ 4 }}}")
t.expect_output_line("stdout*", False)
t.expect_output_line("stderr*", False)
t.expect_nothing_more()

t.run_build_system("-ffile.jam -sXXX=5 -p3", stderr="stderr\n")
t.expect_output_line("{{{ 5 }}}")
t.expect_output_line("stdout")
t.expect_output_line("stderr", False)
t.expect_nothing_more()

t.cleanup()
