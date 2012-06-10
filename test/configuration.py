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
local test-index = [ MATCH ---test-id---=(.*) : [ modules.peek : ARGV ] ] ;
ECHO test-index: $(test-index:E=(unknown)) ;
using %s ;""" % toolsetName)

    class LocalTester:
        def __init__(self, tester):
            self.__tester = tester
            self.__test_ids = []

        def __call__(self, test_id, env, extra_args="", *args, **kwargs):
            if env == "" and not canSetEmptyEnvironmentVariable:
                self.__assertionFailure("Can not set empty environment "
                    "variables on this platform.")
            self.__registerTestId(str(test_id))
            extra_args += " ---test-id---={}".format(test_id)
            env_name = "BOOST_BUILD_USER_CONFIG"
            previous_env = os.environ.pop(env_name, None)
            if env is not None:
                os.environ[env_name] = env
            try:
                t.run_build_system(extra_args, *args, **kwargs)
            finally:
                if previous_env is None:
                    os.environ.pop(env_name, None)
                else:
                    os.environ[env_name] = previous_env

        def __assertionFailure(self, message):
            BoostBuild.annotation("failure", "Internal test assertion failure "
                "- {}".format(message))
            self.__tester.fail_test(1)

        def __registerTestId(self, test_id):
            if test_id in self.__test_ids:
                self.__assertionFailure("Multiple test cases encountered "
                    "using the same test id '{}'.".format(test_id))
            self.__test_ids.append(test_id)

    test = LocalTester(t)

    test(1, None)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(2, None, "--user-config=")
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(3, None, '--user-config=""')
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(4, None, '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(5, None, '--user-config="%s"' % configFileNames[2])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2])

    test(6, None, '--user-config="%s"' % os.path.abspath(configFileNames[1]))
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1])
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(7, None, '--user-config="%s"' % os.path.abspath(configFileNames[2]))
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2])

    if canSetEmptyEnvironmentVariable:
        test(8, "")
        t.expect_output_line(implicitConfigLoadMessage, False)
        t.expect_output_line(explicitConfigLoadMessage, False)
        t.expect_output_line(disabledConfigLoadMessage, True)
        t.expect_output_line(testMessage % configFileNames[0], False)
        t.expect_output_line(testMessage % configFileNames[1], False)
        t.expect_output_line(testMessage % configFileNames[2])

    test(9, '""')
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(10, configFileNames[1])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1])
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(11, configFileNames[1], '--user-config=""')
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage, False)
    t.expect_output_line(disabledConfigLoadMessage)
    t.expect_output_line(testMessage % configFileNames[0], False)
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(12, configFileNames[1], '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    if canSetEmptyEnvironmentVariable:
        test(13, "", '--user-config="%s"' % configFileNames[0])
        t.expect_output_line(implicitConfigLoadMessage, False)
        t.expect_output_line(explicitConfigLoadMessage)
        t.expect_output_line(disabledConfigLoadMessage, False)
        t.expect_output_line(testMessage % configFileNames[0])
        t.expect_output_line(testMessage % configFileNames[1], False)
        t.expect_output_line(testMessage % configFileNames[2], False)

    test(14, '""', '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    test(15, "invalid", '--user-config="%s"' % configFileNames[0])
    t.expect_output_line(implicitConfigLoadMessage, False)
    t.expect_output_line(explicitConfigLoadMessage)
    t.expect_output_line(disabledConfigLoadMessage, False)
    t.expect_output_line(testMessage % configFileNames[0])
    t.expect_output_line(testMessage % configFileNames[1], False)
    t.expect_output_line(testMessage % configFileNames[2], False)

    t.cleanup()


def _canSetEmptyEnvironmentVariable():
    """
      Unfortunately different OSs (and possibly Python implementations as well)
    have different interpretations of what it means to set an evironment
    variable to an empty string. Some (e.g. Windows) interpret it as unsetting
    the variable and some (e.g. AIX or Darwin) actually set it to an empty
    string.

    """
    dummyName = "UGNABUNGA_FOO_BAR_BAZ_FEE_FAE_FOU_FAM"
    original = os.getenv(dummyName)
    os.putenv(dummyName, "")
    result = os.getenv(dummyName) == ""
    if original is None:
        os.unsetenv(dummyName)
    else:
        os.putenv(dummyName)
    return result


################################################################################
#
# main()
# ------
#
################################################################################

canSetEmptyEnvironmentVariable = _canSetEmptyEnvironmentVariable()

test_user_configuration()
