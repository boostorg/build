#!/usr/bin/python

# Copyright 2002 Dave Abrahams
# Copyright 2003, 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild
import os
import os.path
import re


def match_re(actual, expected):
    return re.match(expected, actual, re.DOTALL) != None


t = BoostBuild.Tester(match=match_re, boost_build_path='', pass_toolset=0)
t.set_tree('startup')

t.run_build_system(status=1, stdout=
r'''Unable to load Boost\.Build: could not find "boost-build\.jam"
.*Attempted search from .* up to the root''')

t.run_build_system(status=1, subdir='no-bootstrap1',
    stdout=r'''Unable to load Boost\.Build: could not find build system\.'''
    r'''.*attempted to load the build system by invoking'''
    r'''.*'boost-build ;'.*'''
    r'''but we were unable to find "bootstrap\.jam"''')

# Descend to a subdirectory which /does not/ contain a boost-build.jam file,
# and try again to test the crawl-up behavior.
t.run_build_system(status=1, subdir=os.path.join('no-bootstrap1', 'subdir'),
    stdout=r'''Unable to load Boost\.Build: could not find build system\.'''
    r'''.*attempted to load the build system by invoking'''
    r'''.*'boost-build ;'.*'''
    r'''but we were unable to find "bootstrap\.jam"''')

t.run_build_system(status=1, subdir='no-bootstrap2',
    stdout=r'''Unable to load Boost\.Build: could not find build system\.'''
    r'''.*attempted to load the build system by invoking'''
    r'''.*'boost-build \. ;'.*'''
    r'''but we were unable to find "bootstrap\.jam"''')

t.run_build_system(status=1, subdir='no-bootstrap3', stdout=
r'''Unable to load Boost.Build
.*boost-build\.jam" was found.*
However, it failed to call the "boost-build" rule''')

# Test bootstrapping based on BOOST_BUILD_PATH.
t.run_build_system(subdir='bootstrap-env',
    extra_args='-sBOOST_BUILD_PATH=../boost-root/build',
    stdout='build system bootstrapped')

# Test bootstrapping based on an explicit path in boost-build.jam.
t.run_build_system(subdir='bootstrap-explicit',
    stdout='build system bootstrapped')

t.cleanup()
