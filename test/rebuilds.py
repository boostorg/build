#!/usr/bin/python

# Copyright 2005 Dave Abrahams
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild

t = BoostBuild.Tester(["-ffile.jam", "-d+3", "-d+12", "-d+13"], pass_d0=False,
    pass_toolset=0)

t.write("file.jam", """\
rule make
{
    DEPENDS $(<) : $(>) ;
    DEPENDS all : $(<) ;
}
actions make
{
    echo "******" making $(<) from $(>) "******"
    echo made from $(>) > $(<)
}

make aux1 : bar ;
make foo : bar ;
REBUILDS foo : bar ;
make bar : baz ;
make aux2 : bar ;
""")

t.write("baz", "nothing")

t.run_build_system(["bar"])
t.expect_addition("bar")
t.expect_nothing_more()

t.wait_for_time_change_since_last_build()
t.run_build_system(["foo"])
t.expect_touch("bar")
t.expect_addition("foo")
t.expect_nothing_more()

t.run_build_system()
t.expect_addition(["aux1", "aux2"])
t.expect_nothing_more()

t.touch("bar")
# Wait to make the test system correctly recognize the 'bar' file as touched by
# the next build run, even if it uses 1 second file modification timestamp
# resolution and the build system does the rebuild fast enough that the
# original and the touched files have their file modification timestamps within
# the same second.
t.wait_for_time_change()
t.run_build_system()
t.expect_touch(["foo", "bar", "aux1", "aux2"])
t.expect_nothing_more()

t.cleanup()
