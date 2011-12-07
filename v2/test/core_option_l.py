#!/usr/bin/python

# Copyright 2007 Rene Rivera.
# Copyright 2011 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)

t.write("sleep.bat","""@setlocal
@echo off
timeout /T %1 /NOBREAK >nul
""")

t.write("file.jam", """

actions .a. {
echo 001
sleep 4
echo 002
}

.a. sleeper ;

DEPENDS all : sleeper ;
""")

t.run_build_system("-ffile.jam -d1 -l2", status=1, stdout="""...found 2 targets...
...updating 1 target...
.a. sleeper
2 second time limit exceeded
001

echo 001
sleep 4
echo 002

...failed .a. sleeper...
...failed updating 1 target...
""")

t.cleanup()
