#!/bin/sh

#~ Copyright 2002-2005 Rene Rivera.
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

cd doc
rm -Rf html
bjam --v2
rm -Rf ../src/images ../src/jam ../src/*.html ../src/*.png ../src/*.css ../src/LICENSE*.txt
cp -R html/* ../src
cd ../src
. ./build.sh --- dist
