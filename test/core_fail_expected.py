#!/usr/bin/python

# Copyright 2017 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild

def test_basic():
    t = BoostBuild.Tester(pass_toolset=0)

    t.write("file.jam", """\
    actions fail
    {
        invalid-dd0eeb5899734622
    }

    FAIL_EXPECTED t1 ;
    fail t1 ;

    UPDATE t1 ;
    """)

    t.run_build_system(["-ffile.jam"])

    t.cleanup()
    
def test_error():
    t = BoostBuild.Tester(pass_toolset=0)

    t.write("file.jam", """\
    actions pass
    {
        echo okay
    }

    FAIL_EXPECTED t1 ;
    fail t1 ;

    UPDATE t1 ;
    """)

    t.run_build_system(["-ffile.jam"], status=1)

    t.cleanup()

def test_multiple_actions():
    t = BoostBuild.Tester(pass_toolset=0)

    t.write("file.jam", """\
    actions fail
    {
        invalid-dd0eeb5899734622
    }

    actions pass
    {
        exit 0
    }

    FAIL_EXPECTED t1 ;
    fail t1 ;
    pass t1 ;

    UPDATE t1 ;
    """)

    t.run_build_system(["-ffile.jam"])

    t.cleanup()

test_basic()
test_error()
test_multiple_actions()
