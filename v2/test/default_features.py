#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  Test that features with default values are always present 
#  in build properties of any target.

from BoostBuild import Tester, List

t = Tester()

# Declare *non-propagated* feature foo.
t.write("project-root.jam", """
import feature : feature ;

feature foo : on off ;
""")

# Note that '<foo>on' won't be propagated
# to 'd/l'.
t.write("Jamfile", """
exe hello : hello.cpp d//l ;
""")
t.write("hello.cpp", """
#ifdef _WIN32
__declspec(dllimport)
#endif
void foo();
int main()
{
   foo();
   return 1;
}

""")

t.write("d/Jamfile", """
lib l : l.cpp : <foo>on:<define>FOO ;
""")
t.write("d/l.cpp", """
#ifdef _WIN32
__declspec(dllexport)
#endif
#ifdef FOO
void foo() {}
#endif

""")

t.run_build_system()

n = t.adjust_names("bin/$toolset/debug/main-target-hello/hello.exe")[0]
n2 = t.adjust_names("bin/$toolset/debug/hello.exe")[0]
added = t.unexpected_difference.added_files
t.fail_test(not (n in added) and not (n2 in added))

t.cleanup()
