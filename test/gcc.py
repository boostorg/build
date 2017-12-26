#!/usr/bin/python
#
# Copyright 2017 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

# validates the gcc toolset using a mock of gcc

import BoostBuild
import os
import re
import sys

renames = {"debug": "variant=debug", "release": "variant=release"}

def set_default_target_os(os):
    global removed
    global default_target_os
    default_target_os = os
    removed = set()
    removed.add("target-os=" + default_target_os)

def adjust_property(property):
    global renames
    if property in renames:
        return renames[property]
    else:
        return property

def adjust_properties(properties):
    global removed
    return [adjust_property(p) for p in properties if p not in removed]

def has_property(name, properties):
    return name in [re.sub("=.*", "", p) for p in properties]

def get_target_os(properties):
    for m in [re.match("(.*)=(.*)", p) for p in properties]:
        if m and m.group(1) == "target-os":
            return m.group(2)
    return default_target_os 

def expand_properties(properties):
    result = properties[:]
    if not has_property("variant", properties):
        result += ["variant=debug"]
    if not has_property("threading", properties):
        result += ["threading=single"]
    if not has_property("link", properties):
        result += ["link=shared"]
    if not has_property("runtime-link", properties):
        result += ["runtime-link=shared"]
    if not has_property("target-os", properties):
        result += ["target-os=" + default_target_os]
    return result

def compute_path(properties):
    path = ""
    if "variant=release" in properties:
        path += "/release"
    else:
        path += "/debug"
    if "threading=multi" in properties:
        path += "/threading-multi"
    if "link=static" in properties:
        path += "/link-static"
    if "runtime-link=static" in properties:
        path += "/runtime-link-static"
    if "target-os=linux" in properties:
        path += "/target-os-linux"
    return path

def test_gcc(version, property_sets):
    t = BoostBuild.Tester()

    t.set_tree("toolset-mock")

    # Build necessary tools
    t.run_build_system(["-sPYTHON_CMD=%s" % sys.executable], subdir="src")
    set_default_target_os(t.read("src/bin/target-os.txt").strip())

    for properties in property_sets:
        t.set_toolset("gcc-" + version, get_target_os(properties))
        properties = adjust_properties(properties)
        path = compute_path(properties)
        os.environ["B2_PROPERTIES"] = " ".join(expand_properties(properties))
        t.run_build_system(["--user-config="] + properties)
        t.expect_addition("bin/gcc-*%s%s/lib.obj" % (version, path))
        if "link=static" not in properties:
            t.expect_addition("bin/gcc-*%s%s/l1.dll" % (version, path))
        else:
            t.expect_addition("bin/gcc-*%s%s/l1.lib" % (version, path))
        t.expect_addition("bin/gcc-*%s%s/main.obj" % (version, path))
        t.expect_addition("bin/gcc-*%s%s/test.exe" % (version, path))
        t.expect_nothing_more()
        t.rm("bin")

    t.cleanup()

test_gcc("4.8.3", [
    ["target-os=linux"],
    ["target-os=linux", "release"],
    ["target-os=linux", "threading=multi"],
    ["target-os=linux", "link=static"],
    ["target-os=linux", "link=static", "runtime-link=static"]])
