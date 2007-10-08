#!/usr/bin/python

#  Copyright (C) Vladimir Prus 2007.
#  Distributed under the Boost Software License, Version 1.0. (See
#  accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

#  Tests that a free feature specified on the command
#  line applies to all targets ever built.
from BoostBuild import Tester, List


t = Tester()

t.write("Jamroot", """
exe hello : hello.cpp foo ;
lib foo : foo.cpp ;
""")
t.write("hello.cpp", """
extern void foo();
#ifdef FOO
int main()
{
    foo();
    return 0;
}
#endif
""")

t.write("foo.cpp", """
#ifdef FOO
#ifdef _WIN32
__declspec(dllexport)
#endif
void foo() {}
#endif
""")

# If FOO is not defined when compiling the 'foo'
# target, we'll get a link error at this point.
t.run_build_system("hello define=FOO")

t.expect_addition("bin/$toolset/debug/hello.exe")

t.cleanup()
