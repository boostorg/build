#!/usr/bin/python

# Copyright 2008 Jurko Gospodnetic
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Tests that generators get selected correctly.

import BoostBuild


################################################################################
#
# test_generator_added_after_already_building_a_target_of_its_target_type()
# -------------------------------------------------------------------------
#
################################################################################

def test_generator_added_after_already_building_a_target_of_its_target_type():
    """Regression test for a Boost Build bug causing it to not use a generator
    if it got added after already building a targer of its target type.
    """

    t = BoostBuild.Tester()

    t.write("dummy.cpp", "void f() {}\n")

    t.write("jamroot.jam", """
# Building this dummy target must not cause a later defined CPP target type
# generator not to be recognized as viable.
obj dummy : dummy.cpp ;
alias the-other-obj : Other//other-obj ;
""")

    t.write("Other/source.extension", "A dummy source file.")

    t.write("Other/mygen.jam", """
import generators ;
import type ;
type.register MY_TYPE : extension ;
generators.register-standard mygen.generate-a-cpp-file : MY_TYPE : CPP ;
rule generate-a-cpp-file { ECHO Generating a CPP file... ; }
actions generate-a-cpp-file { echo void g() {} > "$(<)" }
""")

    t.write("Other/jamfile.jam", """
import mygen ;
obj other-obj : source.extension ;
""")

    t.run_build_system()
    t.expect_output_line("Generating a CPP file...")
    t.expect_addition("bin/$toolset/debug/dummy.obj")
    t.expect_addition("Other/bin/$toolset/debug/other-obj.obj")

    t.cleanup()


################################################################################
#
# main()
# ------
#
################################################################################

test_generator_added_after_already_building_a_target_of_its_target_type()
