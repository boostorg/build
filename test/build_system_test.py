#!/usr/bin/python2.1

# Copyright (C) 2001 Vladimir Prus. Permission to copy, use, modify, sell and
# distribute this software is granted, provided this copyright notice appears
# in all copies and modified versions are clearly marked as such. This software
# is provided "as is" without express or implied warranty, and with no claim as
# to is suitability for any purpose.

from __future__ import nested_scopes;

import re;
import os;
import zipfile;
import types;
import string;
import unittest;
import sys;
import getopt;
import time;


import build_system_test_core;
import boost_build_system;


from build_system_test_utils import *;

toolset = ""
tests = ""

def main():
    parse_command_line()
    if not toolset:
        print "Toolset not specified. "\
              "Use \"--toolset\" option or TOOLS environment variable."
        sys.exit(1)
    else:
        # (re)export to shell, so that tests will use only specified tools
        os.environ['TOOLS'] = toolset

    test_suites = []

    r = re.compile(".+\.zip")
    files = filter(lambda x: r.match(x) != None, os.listdir("."))
    for f in files:
        z = zipfile.ZipFile(f)
        try:
            info = z.getinfo("test.py")
        except KeyError:
            print f, "does not have member called 'test.py' -- skipped."
        llocals = {}
        exec string.replace(z.read("test.py"), "\r", "") in globals(), llocals
        l1 = lambda x: type(llocals[x]) == types.ClassType
        l2 = lambda x: l1(x) and\
             inherited(llocals[x], build_system_test_core.Build_system_tester)
        classes = map(lambda x: llocals[x], filter(l2, llocals.keys()))
        test_cases = []
        for c in classes:
            if c.__dict__.has_key("toolsets") and toolset not in c.toolsets:
                continue
            c.archive = f
            names = unittest.defaultTestLoader.getTestCaseNames(c)
            test_cases.extend(map(c, names))
        ts = unittest.TestSuite()
        ts.addTests(test_cases)
        test_suites.append(ts)


    runner = unittest.TextTestRunner()
    everything = unittest.TestSuite()
    everything.addTests(test_suites)
    runner.run(everything)

def parse_command_line():
    global toolset, tests
    try:
        opts, args = getopt.getopt(sys.argv[1:], "", ["toolset="])
    except getopt.GetoptError, x:
        print x
        usage()
    opts = list2map(opts)
    if opts.has_key("--toolset"):
        toolset = opts["--toolset"]

    if not toolset and os.environ.has_key('TOOLS'):
        tools = string.split(os.environ['TOOLS'])
        if len(tools) > 1:
            print "TOOLS variable: only one toolset can be specified at a time."
            sys.exit(1)
        else:
            toolset = tools[0]

    tests = args


def usage():
    print "usage: build_system_test [--toolset=yfc]"
    sys.exit(1)


main()
