#!/usr/bin/python
#
# Copyright 2017 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

from MockProgram import *

command('ranlib', input_file('bin/gcc-gnu-4.8.3/debug/link-static/libl1.a'))
command('ranlib', input_file('bin/gcc-gnu-4.8.3/debug/link-static/runtime-link-static/libl1.a'))

main()
