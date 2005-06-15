#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2004. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  Tests that on gcc, we correctly report problem when static runtime
#  is requested when building DLL.
from BoostBuild import Tester, List
import string

t = Tester()

# Create the needed files
t.write("project-root.jam", "")
t.write("Jamfile", """
lib hello : hello.cpp ;
""")
t.write("hello.cpp", """
int main()
{
    return 0;
}
""")

t.run_build_system("runtime-link=static", status=1)
t.fail_test(string.find(t.stdout(),
                        "on gcc, DLL can't be build with <runtime-link>static") == -1)

t.run_build_system("link=static runtime-link=static")
t.expect_addition("bin/$toolset/debug/link-static/runtime-link-static/hello.lib")

t.cleanup()
