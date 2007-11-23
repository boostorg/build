#!/usr/bin/python

# Copyright 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#  This file is template for Boost.Build tests. It creates a simple
#  project that builds one exe from one source, and checks that the exe
#  is really created.
from BoostBuild import Tester, List


t = Tester()

t.write("a.cpp", """ 
#ifdef CF_IS_OFF
int main() { return 0; }
#endif

""")

t.write("b.cpp", """ 
#ifdef CF_1
int main() { return 0; }
#endif

""")

t.write("c.cpp", """
#ifdef FOO
int main() { return 0; }
#endif

""")


t.write("Jamfile", """ 
# See if default value of composite feature 'cf'
# will be expanded to <define>CF_IS_OFF
exe a : a.cpp ;

# See if subfeature in requirements in expanded.
exe b : b.cpp : <cf>on-1 ;

# See if conditional requirements are recursively expanded.
exe c : c.cpp : <toolset>$toolset:<variant>release <variant>release:<define>FOO ;
""")

t.write("project-root.jam", """ 
import feature ;

feature.feature cf : off on : composite incidental ;

feature.compose <cf>off : <define>CF_IS_OFF ;

feature.subfeature cf on : version : 1 2 : composite optional incidental ;

feature.compose <cf-on:version>1 : <define>CF_1 ;
        
""")

t.expand_toolset("Jamfile")

t.run_build_system()
t.expect_addition(["bin/$toolset/debug/a.exe",
                   "bin/$toolset/debug/b.exe",
                   "bin/$toolset/release/c.exe",
                   ])

t.rm("bin")

# Test for issue BB60
t.write("test.cpp", """
#include "header.h"
int main() { return 0; }
""")
t.write("Jamfile", """
project
    : requirements <toolset>$toolset:<include>foo
    ;
exe test : test.cpp : <toolset>$toolset ;
""")
t.expand_toolset("Jamfile")
t.write("foo/header.h", """
""")
t.write("project-root.jam", "")
t.run_build_system()
t.expect_addition("bin/$toolset/debug/test.exe")

t.cleanup()

