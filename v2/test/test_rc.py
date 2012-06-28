#!/usr/bin/python

# Copyright 2012 Jurko Gospodnetic
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Tests rc toolset behaviour.

import BoostBuild


def included_resource_newer_than_rc_script():
    """
      When a .rc script file includes another resource file - the resource file
    being newer than the .rc script file should not cause the .rc script file
    to be considered old and force all of its dependents to rebuild.

    """
    # Temporarily added the --debug-configuration flag to help with debugging
    # some detected test failures on different remote Boost library test
    # runners.
    t = BoostBuild.Tester("-d1 --debug-configuration", pass_d0=False)

    t.write("jamroot.jam", """\
ECHO {{{ [ modules.peek : XXX ] [ modules.peek : NOEXEC ] }}} ;
obj xxx : xxx.rc ;
""")
    t.write("xxx.rc", '1 MESSAGETABLE "xxx.bin"\n')
    t.write("xxx.bin", "foo")

    output_line = "*%s*" % t.adjust_suffix("xxx_res.obj")

    def test1(n, expect, noexec=False):
        params = "-sXXX=%d" % n
        if noexec:
            params += " -n -sNOEXEC=NOEXEC"
        t.run_build_system(params)
        t.expect_output_line("*NOEXEC*", noexec)
        t.expect_output_line(output_line, expect)
        if expect and not noexec:
            expect("bin/$toolset/debug/xxx_res.obj")
        t.expect_nothing_more()

    def test(n, expect):
        test1(n, expect, noexec=True)
        test1(n, expect)

    test(1, t.expect_addition)
    test(2, None)
    t.touch("xxx.bin")
    test(3, t.expect_touch)
    test(4, None)

    t.cleanup()


included_resource_newer_than_rc_script()
