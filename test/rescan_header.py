#!/usr/bin/python

# Copyright 2012 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild
import os

t = BoostBuild.Tester()

# Test a header loop that depends on (but does not contain)
# a generated header.
t.write("test.cpp", """
#include "header1.h"
""")

t.write("header1.h", """
#ifndef HEADER1_H
#define HEADER1_H
#include "header2.h"
#endif
""")

t.write("header2.h", """
#ifndef HEADER2_H
#define HEADER2_H
#include "header1.h"
#include "header3.h"
#endif
""")

t.write("header3.in", """
/* empty file */
""")

t.write("Jamroot.jam", """
import common ;

make header3.h : header3.in : @common.copy ;
obj test : test.cpp : <implicit-dependency>header3.h ;

""")

t.run_build_system("-j2")
t.expect_addition("bin/$toolset/debug/header3.h")
t.expect_addition("bin/$toolset/debug/test.obj")
t.expect_nothing_more()

t.rm(".")

# Test a linear sequence of generated headers.
t.write("test.cpp", """
#include "header1.h"
""")

t.write("header1.in", """
#ifndef HEADER1_H
#define HEADER1_H
#include "header2.h"
#endif
""")

t.write("header2.in", """
#ifndef HEADER2_H
#define HEADER2_H
#include "header3.h"
#endif
""")

t.write("header3.in", """
/* empty file */
""")

t.write("Jamroot.jam", """
import common ;

actions copy {
    sleep 1
    cp $(>) $(<)
}

make header1.h : header1.in : @common.copy ;
make header2.h : header2.in : @common.copy ;
make header3.h : header2.in : @common.copy ;
obj test : test.cpp :
  <implicit-dependency>header1.h
  <implicit-dependency>header2.h
  <implicit-dependency>header3.h
  ;
""")

t.run_build_system("-j2 test")
t.expect_addition("bin/$toolset/debug/header1.h")
t.expect_addition("bin/$toolset/debug/header2.h")
t.expect_addition("bin/$toolset/debug/header3.h")
t.expect_addition("bin/$toolset/debug/test.obj")
t.expect_nothing_more()

t.cleanup()
