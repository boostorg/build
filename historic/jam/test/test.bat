@ECHO OFF

REM ~ Copyright 2006 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

set BJAM=..\src\bin.ntx86\bjam
REM ~ set BJAM=..\src\bin.ntx86.debug\bjam
REM ~ set BJAM=bjam-3_1_14
REM ~ set BJAM=bjam

@ECHO ON

%BJAM% -f test.jam
