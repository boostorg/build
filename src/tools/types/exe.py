# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE.txt or copy at https://www.bfgroup.xyz/b2/LICENSE.txt)

from b2.build import type

def register ():
    type.register_type ('EXE', ['exe'], None, ['NT', 'CYGWIN'])
    type.register_type ('EXE', [], None, [])

register ()
