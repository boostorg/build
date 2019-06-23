@ECHO OFF

REM Copyright (C) 2009 Vladimir Prus
REM Copyright 2019 Rene Rivera
REM
REM Distributed under the Boost Software License, Version 1.0.
REM (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)


:b2_bootstrap
ECHO Bootstrapping the build engine
pushd src\engine
call .\build.bat %* > ..\..\bootstrap.log
@ECHO OFF
popd
if exist ".\src\engine\b2.exe" (
   copy .\src\engine\b2.exe . > nul
   goto :b2_built)
goto :b2_failure


:b2_built
ECHO.
ECHO Bootstrapping is done. To build, run:
ECHO.
ECHO     .\b2 --prefix=DIR install
ECHO.
goto :end


:b2_failure
ECHO.
ECHO Failed to bootstrap the build engine
ECHO Please consult bootstrap.log for further diagnostics.
ECHO.
goto :end


:end
exit /b %ERRORLEVEL%
