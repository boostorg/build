#!/usr/bin/python
# Copyright David Abrahams 2005. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# This tests the build step timing facilities.

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)

t.write('file.jam', '''
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

rule record_time ( target source : user : system )
{
    ECHO record_time called: $(target) / $(source) / $(user) / $(system) ;
    __USER_TIME__ on $(target) = $(user) ;
    __SYSTEM_TIME__ on $(target) = $(system) ;
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
''')

import re
t.write('baz', 'nothing\n')
t.run_build_system(
    '-ffile.jam',
    stdout=r'bar +user: [0-9\.]+ +system: +[0-9\.]+ *$',
    match = lambda actual,expected: re.search(expected,actual,re.DOTALL)
    )
t.expect_addition('foo')
t.expect_addition('bar')
t.expect_nothing_more()

t.cleanup()
