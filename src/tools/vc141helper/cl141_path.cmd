:: Copyright 2017 - Refael Ackermann
:: Distributed under the Boost Software License, Version 1.0.
:: (See accompanying file LICENSE_1_0.txt or copy at
:: http://www.boost.org/LICENSE_1_0.txt)
@IF NOT DEFINED DEBUG_GETTER @ECHO OFF
SETLOCAL
PUSHD %~dp0
SET PROMPT=$G
SET DEBUG_GETTER=
CALL :find_CL %~dp0 %1
POPD
GOTO :eof

:find_CL
FOR /F "tokens=*" %%A IN ('powershell -NoProfile -ExecutionPolicy Unrestricted "%1GetCLPath.ps1" %2') DO ECHO %%A& EXIT /B
GOTO :eof