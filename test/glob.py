#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2003. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

#  Test the 'glob' rule in Jamfile context.
from BoostBuild import Tester, List

# Create a temporary working directory
t = Tester()

t.write("project-root.jam", """ 
""")

t.write("Jamfile", """ 
""")

t.write("d1/a.cpp", """ 
int main() { return 0; }

""")

t.write("d1/Jamfile", """ 
exe a : [ glob *.cpp ] ../d2/d//l ; 
""")

t.write("d2/d/l.cpp", """ 
#if defined(_WIN32)
__declspec(dllexport)
void force_import_lib_creation() {}
#endif
""")

t.write("d2/d/Jamfile", """ 
lib l : [ glob *.cpp ] ; 
""")

t.run_build_system(subdir="d1")

n = t.adjust_names("d1/bin/$toolset/debug/main-target-a/a.exe")[0]
n2 = t.adjust_names("d1/bin/$toolset/debug/a.exe")[0]
added = t.unexpected_difference.added_files
t.fail_test(not (n in added) and not (n2 in added))

t.rm("d2/d/bin")
t.run_build_system(subdir="d2/d")
t.expect_addition("d2/d/bin/$toolset/debug/l.dll")


t.cleanup()
