#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

from BoostBuild import Tester, List

t = Tester()

t.write("Jamroot", """
project : requirements <link>static ;
exe a : a.cpp [ lib helper : helper.cpp ] ; 
""")

t.write("a.cpp", """
extern void helper();
int main()
{
    return 0;
}

""")

t.write("helper.cpp", """
void helper()
{
}
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/link-static/a__helper.lib")
t.rm("bin/$toolset/debug/link-static/a__helper.lib")

t.run_build_system("a__helper")
t.expect_addition("bin/$toolset/debug/link-static/a__helper.lib")

t.rm("bin")

# Now check that inline targets with the same name but
# present in different places are not confused between
# each other, and with top-level targets.
t.write("Jamroot", """
project : requirements <link>static ;
exe a : a.cpp [ lib helper : helper.cpp ] ;
exe a2 : a.cpp [ lib helper : helper.cpp ] ; 
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug/link-static/a.exe")
t.expect_addition("bin/$toolset/debug/link-static/a__helper.lib")
t.expect_addition("bin/$toolset/debug/link-static/a2__helper.lib")

# Check that the 'alias' target does not change name of
# inline targets, and that inline targets are explicit.
t.write("Jamroot", """
project : requirements <link>static ;
alias a : [ lib helper : helper.cpp ] ;
explicit a ;
""")
t.rm("bin")

t.run_build_system()
t.expect_nothing_more()

t.run_build_system("a")
t.expect_addition("bin/$toolset/debug/link-static/helper.lib")

t.cleanup()

