# Copyright (C) 2001 Vladimir Prus. Permission to copy, use, modify, sell and
# distribute this software is granted, provided this copyright notice appears
# in all copies and modified versions are clearly marked as such. This software
# is provided "as is" without express or implied warranty, and with no claim as
# to is suitability for any purpose.

import build_system_test_core;

import os;
import string;

def the_runner(extra_options, commands_only):
    # Get the list of commands first
    n = os.tempnam()
    s = os.system("jam -n -d0 -o" + n + " " + extra_options)
    assert s == 0

    f = open(n)
    # Remove empty lines
    lines = filter(lambda x: x, map(string.strip, f.readlines()))
    if commands_only:
        s = 0
    else:
            s = os.system("jam " + extra_options)
    return (s, lines)


class tester(build_system_test_core.Build_system_tester):
    runner = the_runner
