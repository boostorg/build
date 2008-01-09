#!/usr/bin/python
# Copyright David Abrahams 2005. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# This tests the build step timing facilities.

import BoostBuild
import re

t = BoostBuild.Tester(pass_toolset=0)

t.write("file.jam", """
rule time
{
    DEPENDS $(<) : $(>) ;
    __TIMING_RULE__ on $(>) = record_time $(<) ;
    DEPENDS all : $(<) ;
}

actions time
{
    echo $(>) user: $(__USER_TIME__) system: $(__SYSTEM_TIME__)
    echo timed from $(>) >> $(<)
}

rule display-action-output ( args * : target : command status start end user system : output-lines * )
{
    for local line in $(output-lines)
    {
        ECHO $(line) ;
    }
}

rule record_time ( target : source : start end user system )
{
    __USER_TIME__   on $(target) = $(user) ;
    __SYSTEM_TIME__ on $(target) = $(system) ;
    __ACTION_RULE__ on $(target) = display-action-output ;
}

rule make
{
    DEPENDS $(<) : $(>) ;
}

actions make
{
    echo made from $(>) >> $(<)
}

time foo : bar ;
make bar : baz ;
""")

t.write("baz", "nothing\n")
t.run_build_system(
    "-ffile.jam",
    stdout=r"bar +user: [0-9.]+ +system: +[0-9.]+ *$",
    match=lambda actual,expected: re.search(expected,actual,re.DOTALL))
t.expect_addition("foo")
t.expect_addition("bar")
t.expect_nothing_more()

t.cleanup()
