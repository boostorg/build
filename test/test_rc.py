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
    toolsetName = "__myDummyResourceCompilerToolset__"

    # We pass -d4 & --debug-configuration flags so we can get additional
    # information in case this test fails. In the past we have had testing
    # system issues causing this test to fail sporadically for which -d+3
    # output had been instrumental in getting to the root cause (a touched
    # file's timestamp was not as new as it should have been).
    t = BoostBuild.Tester("-d4 toolset=__myDummyResourceCompilerToolset__ "
        "--debug-configuration", pass_d0=False, pass_toolset=False,
        use_test_config=False, translate_suffixes=False)

    # Prepare a dummy toolset so we do not get errors in case the default one
    # is not found and that we can test rc.jam functionality without having to
    # depend on the externally specified toolset actually supporting it.
    t.write(toolsetName + ".jam", """\
import feature ;
import rc ;
local toolset-name = "%s" ;
feature.extend toolset : $(toolset-name) ;
rule init ( ) { }
rc.configure dummy-rc-command : <toolset>$(toolset-name) : <rc-type>dummy ;
module rc
{
    rule compile.resource.dummy ( targets * : sources * : properties * )
    {
        import common ;
        .TOUCH on $(targets) = [ common.file-touch-command ] ;
    }
    actions compile.resource.dummy { $(.TOUCH) "$(<)" }
}
""" % toolsetName)

    # Prepare project source files.
    t.write("jamroot.jam", """\
ECHO {{{ [ modules.peek : XXX ] [ modules.peek : NOEXEC ] }}} ;
obj xxx : xxx.rc ;
""")
    t.write("xxx.rc", '1 MESSAGETABLE "xxx.bin"\n')
    t.write("xxx.bin", "foo")

    def test1(n, expect, noexec=False):
        params = "-sXXX=%d" % n
        if noexec:
            params += " -n -sNOEXEC=NOEXEC"
        t.run_build_system(params)
        t.expect_output_line("*NOEXEC*", noexec)
        t.expect_output_line("compile.resource.dummy *xxx_res.obj", expect)
        if expect and not noexec:
            expect("bin/%s/debug/xxx_res.obj" % toolsetName)
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
