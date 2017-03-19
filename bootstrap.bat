@ECHO OFF

REM Copyright (C) 2009 Vladimir Prus
REM
REM Distributed under the Boost Software License, Version 1.0.
REM (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

ECHO Bootstrapping the build engine
if exist ".\src\engine\bin.ntx86\bjam.exe" del src\engine\bin.ntx86\bjam.exe
if exist ".\src\engine\bin.ntx86_64\bjam.exe" del src\engine\bin.ntx86_64\bjam.exe

SETLOCAL
pushd src\engine
REM Reduce the prompt for a cleaner log
SET PROMPT=$S
cmd /d /c .\build.bat %* > ..\..\bootstrap.log
popd
ENDLOCAL
REM Clear ERRORLEVEL
ver > nul

if exist ".\src\engine\bin.ntx86\b2.exe" (
   copy .\src\engine\bin.ntx86\b2.exe . > nul
   copy .\src\engine\bin.ntx86\bjam.exe . > nul
   goto :bjam_built)

if exist ".\src\engine\bin.ntx86_64\b2.exe" (
   copy .\src\engine\bin.ntx86_64\b2.exe . > nul
   copy .\src\engine\bin.ntx86_64\bjam.exe . > nul
   goto :bjam_built)

goto :bjam_failure

:bjam_built

REM Clear ERRORLEVEL
ver > nul
ECHO.
ECHO Bootstrapping is done. To build, run:
ECHO.
ECHO     .\b2 --prefix=DIR install
ECHO.

goto :end

:bjam_failure

ECHO.
ECHO Failed to bootstrap the build engine
ECHO Please consult bootstrap.log for furter diagnostics.
ECHO.


goto :end

:end
exit /b %ERRORLEVEL%
