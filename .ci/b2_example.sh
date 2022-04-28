#!/bin/sh

set -e

pushd $1
$HOME/temp/.b2/bin/b2 -d+2 ${B2_ARGS}
find . -perm -111 -type f -print
find . -name "*.o" -print
find . -name "*.a" -o -print
find . -name "*.so" -o -print
find . -name "*.dylib" -print
popd

# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt)
#
# Copyright Rene Rivera 2022.
