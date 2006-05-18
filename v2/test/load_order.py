#!/usr/bin/python

# Copyright 2004 Vladimir Prus.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

#  Test that we load parent projects before loading children.
from BoostBuild import Tester, List
from string import find

t = Tester()

t.write("Jamfile", """ 
use-project /child : child ;

ECHO "Setting parent requirements" ;
project
    : requirements <define>PASS_THE_TEST
    ; 
    
alias x : child//main ;    
""")

t.write("project-root.jam", """ 
""")

t.write("child/Jamfile", """ 
ECHO "Setting child requirements" ;
project /child ;

exe main : main.cpp ; 
""")

t.write("child/main.cpp", """ 
#if defined(PASS_THE_TEST)
int main() { return 0; }
#endif

""")

t.run_build_system()


t.expect_addition("child/bin/$toolset/debug/main.exe")
t.fail_test(find(t.stdout(), "Setting child requirements") <
            find(t.stdout(), "Setting parent requirements"))


# Regression test: parent requirements were ignored in some cases
t.rm(".")
t.write("Jamroot", """ 
build-project src ; 
""")

t.write("src/Jamfile", """ 
project : requirements <define>EVERYTHING_OK ; 
""")

t.write("src/app/Jamfile", """ 
exe test : test.cpp ; 
""")

t.write("src/app/test.cpp", """ 
#ifdef EVERYTHING_OK
int main() {}
#endif

""")

t.run_build_system(subdir="src/app")
t.expect_addition("src/app/bin/$toolset/debug/test.exe")


t.cleanup()
