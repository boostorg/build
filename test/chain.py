#!/usr/bin/python

# This tests that 
# 1) the 'make' correctly assigns types to produced targets
# 2) than if 'make' create targets of type CPP, they are
# correctly used (there was a bug with it).

from BoostBuild import Tester
t = Tester()

# In order to correctly link this app, 'b.cpp', created by 'make'
# rule, should be compiled.
t.write("project-root.jam", "import gcc ;")
t.write("Jamfile", """
rule create ( dst : src * : properties * )
{
}

actions create 
{
    echo "int main(){}" > $(<)
}
IMPORT $(__name__) : create : : create ;

exe a : l ;
lib l : a.cpp b.cpp ;

make b.cpp : : create ;

""")
t.write("a.cpp", "")

t.run_build_system()
t.expect_addition("bin/gcc/debug/a")

t.cleanup()
