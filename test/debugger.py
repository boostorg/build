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

rule f ( i )
{
    return $(i) ;
}

rule g ( i )
{
    local result = [ f i ] ;
    return $(result) ;
}

g 1 ;
UPDATE ;
""");

t.run_build_system(stdin="""\
break test.jam:4
break f
run -ftest.jam
print $(arg)
backtrace
continue
finish
step
next
clear test.jam:4
run -ftest.jam
kill
quit
""",
stdout=r"""\(b2db\) Breakpoint 1 set at test.jam:4
\(b2db\) Breakpoint 2 set at f
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, test \( v \) at test\.jam:4
4	    return \$\(arg\) ;
\(b2db\) v
\(b2db\) #0  in test \( v \) at test\.jam:4
#1  in module scope at test\.jam:7
\(b2db\) Breakpoint 2, f \( i \) at test\.jam:11
11	    return \$\(i\) ;
\(b2db\) 17	    return \$\(result\) ;
\(b2db\) 21	UPDATE ;
\(b2db\) Child \d+ exited with status 0
\(b2db\) Deleted breakpoint 1
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 2, f \( i \) at test\.jam:11
11	    return \$\(i\) ;
\(b2db\) \(b2db\) """, match=TestCmd.match_re);

t.cleanup()
