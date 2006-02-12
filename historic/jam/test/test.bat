@ECHO OFF

REM ~ Copyright 2006 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

set BJAM=..\src\bin.ntx86\bjam

@ECHO ON

%BJAM% -f builtin_shell.jam
