#!/usr/bin/python

# Copyright 2008, 2012 Jurko Gospodnetic
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test that Boost Build configuration file handling.

import BoostBuild
import os
import os.path


################################################################################
#
# test_user_configuration()
# -------------------------
#
################################################################################

def test_user_configuration():
    """Test Boost Build user configuration handling. Both relative and absolute
    path handling is tested.
    """

    t = BoostBuild.Tester("--debug-configuration", pass_toolset=False,
        use_test_config=False)

    implicitConfigLoadMessage = "notice: Loading user-config configuration file: *"
    explicitConfigLoadMessage = "notice: Loading explicitly specified user configuration file:"
    disabledConfigLoadMessage = "notice: User configuration file loading explicitly disabled."
    testMessage = "_!_!_!_!_!_!_!_!_ %s _!_!_!_!_!_!_!_!_"
    toolsetName = "__myDummyToolset__"
    subdirName = "ASubDirectory"
    configFileNames = ["ups_lala_1.jam", "ups_lala_2.jam",
        os.path.join(subdirName, "ups_lala_3.jam")]

    for configFileName in configFileNames:
        message = "ECHO \"%s\" ;" % testMessage % configFileName
        # We need to double any backslashes in the message or Jam will interpret
        # them as escape characters.
        t.write(configFileName, message.replace("\\", "\\\\"))

    # Prepare a dummy toolset so we do not get errors in case the default one is
    # not found.
    t.write(toolsetName + ".jam", """\
import feature ;
feature.extend toolset : %s ;
rule init ( ) { }
""" % toolsetName )

    # Python version of the same dummy toolset.
    t.write(toolsetName + ".py", """\
from b2.build import feature
feature.extend('toolset', ['%s'])
def init(): pass
""" % toolsetName )

    t.write("jamroot.jam", """\
local test-index = [ MATCH ---test-index---=(.*) : [ modules.peek : ARGV ] ] ;
ECHO test-index: $(test-index:E=(unknown)) ;
using %s ;""" % toolsetName)

    class CountingTester:
        def __init__(self):
            self.__test_counter = 0

        def __call__(self, env, extra_args="", *args, **kwargs):
            env_name = "BOOST_BUILD_USER_CONFIG"
            previous_env = os.environ.pop(env_name, None)
            if env is not None:
                os.environ[env_name] = env
            try:
                self.__test_counter += 1
                if extra_args:
                    extra_args += " "
                extra_args += "---test-index---={}".format(self.__test_counter)
                t.run_build_system(extra_args, *args, **kwargs)
            finally:
                if previous_env is None:
                    os.environ.pop(env_name, None)
                else:
                    os.environ[env_name] = previous_env

    test = CountingTester()

    test(None)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(None, "--user-config=")
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(None, '--user-config=""')
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(None, '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(None, '--user-config="%s"' % configFileNames[2])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2])

    test(None, '--user-config="%s"' % os.path.abspath(configFileNames[1]))
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1])
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(None, '--user-config="%s"' % os.path.abspath(configFileNames[2]))
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2])

    test("")
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test('""')
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(configFileNames[1])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1])
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(configFileNames[1], '--user-config=""')
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(configFileNames[1], '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test("", '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test("\"\"", '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test("invalid", '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    t.cleanup()


################################################################################
#
# main()
# ------
#
################################################################################

test_user_configuration()
