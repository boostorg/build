#!/bin/sh

#~ Copyright 2002-2005 Rene Rivera.
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

BJAM=`ls -1 ../src/bin.*/bjam`

${BJAM} -f builtin_shell.jam
