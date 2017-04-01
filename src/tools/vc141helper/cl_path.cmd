:: Copyright 2017 - Refael Ackermann
:: Copyright 2017 - Arkady Shapkin
:: Distributed under the Boost Software License, Version 1.0.
:: (See accompanying file LICENSE_1_0.txt or copy at
:: http://www.boost.org/LICENSE_1_0.txt)
@ECHO OFF

FOR /F "tokens=*" %%A IN ('powershell -NoProfile -ExecutionPolicy Bypass "%~dp0GetCLPath.ps1" %1 %2') DO (
    ECHO %%A
    exit /b 0
)
exit /b 1
