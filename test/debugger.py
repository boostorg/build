#!/usr/bin/python

# Copyright 2016 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test for the debugger

import BoostBuild
import TestCmd

t = BoostBuild.Tester(["-dconsole"], pass_toolset=False, pass_d0=False,
    use_test_config=False, ignore_toolset_requirements=False)

t.write("test.jam", """\
rule test ( arg )
{
    local x = $(arg) ;
    return $(arg) ;
}

test v ;
""");

t.run_build_system(stdin="""\
break test.jam:4
run -ftest.jam
print $(arg)
continue
quit
""",
stdout=r"""\(b2db\) Breakpoint 1 set at test.jam:4
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, test \( v \) at test\.jam:4
4	    return \$\(arg\) ;
\(b2db\) v
\(b2db\) don't know how to make all
\.\.\.found 1 target\.\.\.
\.\.\.can't find 1 target\.\.\.
Child \d+ exited with status 1
\(b2db\) """, match=TestCmd.match_re);

t.cleanup()
