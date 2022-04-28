#!/bin/sh

set -e

pushd $1
$HOME/temp/.b2/bin/b2 -d+2 ${B2_ARGS}
echo "-----"
find . -perm -111 -type f
echo "-----"
find . -name "*.o"
echo "-----"
find . -name "*.a" -o
echo "-----"
find . -name "*.so" -o
echo "-----"
find . -name "*.dylib"
echo "-----"
popd

# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt)
#
# Copyright Rene Rivera 2022.
