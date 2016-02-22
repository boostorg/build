#!/usr/bin/python

# Copyright 2016 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test for the debugger

import BoostBuild
import TestCmd
import re

def split_stdin_stdout(text):
    """stdin is all text after the prompt up to and including
    the next newline.  Everything else is stdout.  stdout
    may contain regular expressions enclosed in {{}}."""
    prompt = re.escape('(b2db) ')
    pattern = re.compile('(?<=%s)(.*\n)' % prompt)
    stdin = ''.join(re.findall(pattern, text))
    stdout = re.sub(pattern, '', text)
    outside_pattern = re.compile(r'(?:\A|(?<=\}\}))(?:[^\{]|(?:\{(?!\{)))*(?:(?=\{\{)|\Z)')

    def escape_line(line):
        line = re.sub(outside_pattern, lambda m: re.escape(m.group(0)), line)
        return re.sub(r'\{\{|\}\}', '', line)

    stdout = '\n'.join([escape_line(line) for line in stdout.split('\n')])
    return (stdin,stdout)

def run(tester, io):
    (input,output) = split_stdin_stdout(io)
    tester.run_build_system(stdin=input, stdout=output, match=TestCmd.match_re)

def make_tester():
    return BoostBuild.Tester(["-dconsole"], pass_toolset=False, pass_d0=False,
        use_test_config=False, ignore_toolset_requirements=False, match=TestCmd.match_re)

def test_run():
    t = make_tester()
    t.write("test.jam", """\
        UPDATE ;
    """)

    run(t, """\
(b2db) run -ftest.jam
Starting program: {{.*}}bjam -ftest.jam
Child {{\d+}} exited with status 0
(b2db) quit
""")

    t.cleanup()

def test_exit_status():
    t = make_tester()
    t.write("test.jam", """\
        EXIT : 1 ;
    """)
    run(t, """\
(b2db) run -ftest.jam
Starting program: {{.*}}bjam -ftest.jam

Child {{\d+}} exited with status 1
(b2db) quit
""")
    t.cleanup()

def test_step():
    t = make_tester()
    t.write("test.jam", """\
        rule g ( )
        {
            a = 1 ;
            b = 2 ;
        }
        rule f ( )
        {
            g ;
            c = 3 ;
        }
        f ;
    """)
    t.run_build_system(stdin="""\
        break f
        run -ftest.jam
        step
        step
        step
        quit
    """, stdout=r"""\(b2db\) Breakpoint 1 set at f
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, f \( \) at test.jam:8
8	            g ;
\(b2db\) 3	            a = 1 ;
\(b2db\) 4	            b = 2 ;
\(b2db\) 9	            c = 3 ;
\(b2db\) """)
    t.cleanup()

def test_next():
    t = make_tester()
    t.write("test.jam", """\
        rule g ( )
        {
            a = 1 ;
        }
        rule f ( )
        {
            g ;
            b = 2 ;
            c = 3 ;
        }
        rule h ( )
        {
            f ;
            g ;
        }
        h ;
        d = 4 ;
    """)
    t.run_build_system(stdin="""\
        break f
        run -ftest.jam
        next
        next
        next
        next
        quit
    """, stdout=r"""\(b2db\) Breakpoint 1 set at f
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, f \( \) at test.jam:7
7	            g ;
\(b2db\) 8	            b = 2 ;
\(b2db\) 9	            c = 3 ;
\(b2db\) 14	            g ;
\(b2db\) 17	        d = 4 ;
\(b2db\) """)
    t.cleanup()

def test_finish():
    t = make_tester()
    t.write("test.jam", """\
        rule f ( )
        {
            a = 1 ;
        }
        rule g ( )
        {
            f ;
            b = 2 ;
            i ;
        }
        rule h ( )
        {
            g ;
            i ;
        }
        rule i ( )
        {
            c = 3 ;
        }
        h ;
        d = 4 ;
    """)
    t.run_build_system(stdin="""\
        break f
        run -ftest.jam
        finish
        finish
        finish
        quit
    """, stdout=r"""\(b2db\) Breakpoint 1 set at f
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, f \( \) at test.jam:3
3	            a = 1 ;
\(b2db\) 8	            b = 2 ;
\(b2db\) 14	            i ;
\(b2db\) 21	        d = 4 ;
\(b2db\) """)
    t.cleanup()
    
def test_breakpoints():
    """Tests the interaction between the following commands:
    break, clear, delete, disable, enable"""
    t = make_tester()
    t.write("test.jam", """\
        rule f ( )
        {
            a = 1 ;
        }
        rule g ( )
        {
            b = 2 ;
        }
        rule h ( )
        {
            c = 3 ;
            d = 4 ;
        }
        f ;
        g ;
        h ;
        UPDATE ;
    """)
    t.run_build_system(stdin="""\
        break f
        run -ftest.jam
        kill
        break g
        disable 1
        run -ftest.jam
        kill
        enable 1
        run -ftest.jam
        kill
        delete 1
        run -ftest.jam
        kill
        break test.jam:12
        clear g
        run -ftest.jam
        kill
        clear test.jam:12
        run -ftest.jam
        quit
    """, stdout=r"""\(b2db\) Breakpoint 1 set at f
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, f \( \) at test.jam:3
3	            a = 1 ;
\(b2db\) \(b2db\) Breakpoint 2 set at g
\(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 2, g \( \) at test.jam:7
7	            b = 2 ;
\(b2db\) \(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, f \( \) at test.jam:3
3	            a = 1 ;
\(b2db\) \(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 2, g \( \) at test.jam:7
7	            b = 2 ;
\(b2db\) \(b2db\) Breakpoint 3 set at test\.jam:12
\(b2db\) Deleted breakpoint 2
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 3, h \( \) at test.jam:12
12	            d = 4 ;
\(b2db\) \(b2db\) Deleted breakpoint 3
\(b2db\) Starting program: .*bjam -ftest\.jam
Child \d+ exited with status 0
\(b2db\) """)
    t.cleanup()
    
def test_breakpoints_running():
    """Tests that breakpoints can be added and modified
    while the program is running."""
    t = make_tester()
    t.write("test.jam", """\
        rule f ( )
        {
            a = 1 ;
        }
        rule g ( )
        {
            b = 2 ;
        }
        rule h ( )
        {
            c = 3 ;
            d = 4 ;
        }
        f ;
        g ;
        h ;
        UPDATE ;
    """)
    t.run_build_system(stdin="""\
        break test.jam:14
        run -ftest.jam
        break f
        continue
        kill
        run -ftest.jam
        break g
        disable 2
        continue
        kill
        run -ftest.jam
        enable 2
        continue
        kill
        run -ftest.jam
        delete 2
        continue
        kill
        run -ftest.jam
        break test.jam:12
        clear g
        continue
        kill
        run -ftest.jam
        clear test.jam:12
        continue
        quit
    """, stdout=r"""\(b2db\) Breakpoint 1 set at test.jam:14
\(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, module scope at test\.jam:14
14	        f ;
\(b2db\) Breakpoint 2 set at f
\(b2db\) Breakpoint 2, f \( \) at test\.jam:3
3	            a = 1 ;
\(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, module scope at test\.jam:14
14	        f ;
\(b2db\) Breakpoint 3 set at g
\(b2db\) \(b2db\) Breakpoint 3, g \( \) at test\.jam:7
7	            b = 2 ;
\(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, module scope at test\.jam:14
14	        f ;
\(b2db\) \(b2db\) Breakpoint 2, f \( \) at test\.jam:3
3	            a = 1 ;
\(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, module scope at test\.jam:14
14	        f ;
\(b2db\) \(b2db\) Breakpoint 3, g \( \) at test\.jam:7
7	            b = 2 ;
\(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, module scope at test\.jam:14
14	        f ;
\(b2db\) Breakpoint 4 set at test\.jam:12
\(b2db\) Deleted breakpoint 3
\(b2db\) Breakpoint 4, h \( \) at test\.jam:12
12	            d = 4 ;
\(b2db\) \(b2db\) Starting program: .*bjam -ftest\.jam
Breakpoint 1, module scope at test\.jam:14
14	        f ;
\(b2db\) Deleted breakpoint 4
\(b2db\) Child \d+ exited with status 0
\(b2db\) """)
    t.cleanup()

def test_backtrace():
    t = make_tester()
    t.write("test.jam", """\
        rule f ( x * : y * : z * )
        {
            return $(x) ;
        }
        rule g ( x * : y * : z * )
        {
            return [ f $(x) : $(y) : $(z) ] ;
        }
        g 1 : 2 : 3 ;
    """)
    run(t, """\
(b2db) break f
Breakpoint 1 set at f
(b2db) run -ftest.jam
Starting program: {{.*}}bjam -ftest.jam
Breakpoint 1, f ( 1 : 2 : 3 ) at test.jam:3
3	            return $(x) ;
(b2db) backtrace
#0  in f ( 1 : 2 : 3 ) at test.jam:3
#1  in g ( 1 : 2 : 3 ) at test.jam:7
#2  in module scope at test.jam:9
(b2db) quit
""")
    t.cleanup()

test_run()
test_exit_status()
test_step()
test_next()
test_finish()
test_breakpoints()
test_breakpoints_running()
test_backtrace()
