@ECHO OFF

REM ~ Copyright 2006 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

setlocal

cd doc
del /F /S /Q html
bjam --v2
del /F /S /Q ..\src\images ..\src\jam ..\src\*.html ..\src\*.png ..\src\*.css ..\src\LICENSE*.txt
xcopy /S /I /Y html ..\src
cd ..\src
call .\build.bat --- dist
