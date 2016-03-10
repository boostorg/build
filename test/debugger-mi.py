#!/usr/bin/python

# Copyright 2016 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Test the mi interface for the debugger

import BoostBuild
import TestCmd
import re

def split_stdin_stdout(text):
    """stdin is all text after the prompt up to and including
    the next newline.  Everything else is stdout.  stdout
    may contain regular expressions enclosed in {{}}."""
    prompt = re.escape('(gdb) \n')
    pattern = re.compile('(?<=%s)((?:[^\*].*)?\n)' % prompt)
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
    return BoostBuild.Tester(["-dmi"], pass_toolset=False, pass_d0=False,
        use_test_config=False, ignore_toolset_requirements=False, match=TestCmd.match_re)

def test_exec_run():
    t = make_tester()
    t.write("test.jam", """\
        UPDATE ;
    """)

    run(t, """\
=thread-group-added,id="i1"
(gdb) 
72-exec-run -ftest.jam
=thread-created,id="1",group-id="i1"
72^running
(gdb) 
*stopped,reason="exited-normally"
(gdb) 
73-gdb-exit
73^exit
""")

    t.cleanup()

test_exec_run()
