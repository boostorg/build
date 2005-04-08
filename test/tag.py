#!/usr/bin/python

#  Copyright (C) Pedro Ferreira 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

from BoostBuild import Tester, List
import string

t = Tester()

t.write("project-root.jam", "")
t.write("Jamfile", """ 
import virtual-target ;
rule tag ( name : type ? : property-set )
{
    local tags ;
    local v = [ $(property-set).get <variant> ] ;
    if $(v) = debug
    {
        tags += d ;
    }
    else if $(v) = release
    {
	tags += r ;
    }
    
    local l = [ $(property-set).get <link> ] ;
    if $(l) = shared
    {
	tags += s ;
    }
    else if $(l) = static
    {
	tags += t ;
    }
    
    if $(tags)
    {
	return [ virtual-target.add-suffix $(name)_$(tags:J="") 
 	    : $(type) : $(property-set) ] ;
    }
    
}

exe a : a.cpp : <tag>@$(__name__).tag ;
lib b : a.cpp : <tag>@$(__name__).tag ;
stage c : a ;
""")

t.write("a.cpp", """ 
int main()
{
    return 0;
}

#ifdef _MSC_VER
__declspec (dllexport) void x () {} 
#endif
""")

file_list = \
List("bin/$toolset/debug/a_ds.exe") + \
List("bin/$toolset/debug/b_ds.dll") + \
List("c/a_ds.exe") + \
List("bin/$toolset/release/a_rs.exe") + \
List("bin/$toolset/release/b_rs.dll") + \
List("c/a_rs.exe") + \
List("bin/$toolset/debug/link-static/a_dt.exe") + \
List("bin/$toolset/debug/link-static/b_dt.lib") + \
List("c/a_dt.exe") + \
List("bin/$toolset/release/link-static/a_rt.exe") + \
List("bin/$toolset/release/link-static/b_rt.lib") + \
List("c/a_rt.exe")

variants = "debug release link=static,shared"

t.run_build_system(variants)
t.expect_addition(file_list)

t.run_build_system(variants + " clean")
t.expect_removal(file_list)

# Regression test: the 'tag' feature did not work in directories that
# had dot in names.
t.write("version-1.32.0/Jamroot", """
project test : requirements <tag>@$(__name__).tag ;

rule tag ( name : type ? : property-set )
{
   # Do nothing, just make sure the rule is invoked OK.
   ECHO "The tag rule was invoked" ;
}
exe a : a.cpp ;
""")

t.write("version-1.32.0/a.cpp", "int main() { return 0; }\n")

t.run_build_system(subdir="version-1.32.0")
t.expect_addition("version-1.32.0/bin/$toolset/debug/a.exe")
t.fail_test(string.find(t.stdout(), "The tag rule was invoked") == -1)

t.cleanup()

