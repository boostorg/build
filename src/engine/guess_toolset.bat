@ECHO OFF

REM ~ Copyright 2002-2018 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

call :Guess


:Clear_Error
ver >nul
goto :eof


:Test_Path
REM Tests for the given executable file presence in the directories in the PATH
REM environment variable. Additionally sets FOUND_PATH to the path of the
REM found file.
call :Clear_Error
setlocal
set test=%~$PATH:1
endlocal
if not errorlevel 1 set FOUND_PATH=%~dp$PATH:1
goto :eof


:Guess
REM Let vswhere tell us where msvc is at, if available.
call :Clear_Error
call vswhere_usability_wrapper.cmd
if NOT "_%VS150COMNTOOLS%_" == "__" (
    set "BOOST_JAM_TOOLSET=vc141"
    set "BOOST_JAM_TOOLSET_ROOT=%VS150COMNTOOLS%..\..\VC\"
    exit /b 0)

call :Clear_Error
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "BOOST_JAM_TOOLSET=vc141"
    set "BOOST_JAM_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio\2017\Enterprise\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "BOOST_JAM_TOOLSET=vc141"
    set "BOOST_JAM_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio\2017\Professional\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "BOOST_JAM_TOOLSET=vc141"
    set "BOOST_JAM_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio\2017\Community\VC\"
    exit /b 0)
if NOT "_%VS140COMNTOOLS%_" == "__" (
    set "BOOST_JAM_TOOLSET=vc14"
    set "BOOST_JAM_TOOLSET_ROOT=%VS140COMNTOOLS%..\..\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio 14.0\VC\VCVARSALL.BAT" (
    set "BOOST_JAM_TOOLSET=vc14"
    set "BOOST_JAM_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio 14.0\VC\"
    exit /b 0)
if NOT "_%VS120COMNTOOLS%_" == "__" (
    set "BOOST_JAM_TOOLSET=vc12"
    set "BOOST_JAM_TOOLSET_ROOT=%VS120COMNTOOLS%..\..\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio 12.0\VC\VCVARSALL.BAT" (
    set "BOOST_JAM_TOOLSET=vc12"
    set "BOOST_JAM_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio 12.0\VC\"
    exit /b 0)
if NOT "_%VS110COMNTOOLS%_" == "__" (
    set "BOOST_JAM_TOOLSET=vc11"
    set "BOOST_JAM_TOOLSET_ROOT=%VS110COMNTOOLS%..\..\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio 11.0\VC\VCVARSALL.BAT" (
    set "BOOST_JAM_TOOLSET=vc11"
    set "BOOST_JAM_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio 11.0\VC\"
    exit /b 0)
call :Test_Path cl.exe
if not errorlevel 1 (
    set "BOOST_JAM_TOOLSET=msvc"
    set "BOOST_JAM_TOOLSET_ROOT=%FOUND_PATH%..\"
    exit /b 0)
call :Test_Path vcvars32.bat
if not errorlevel 1 (
    set "BOOST_JAM_TOOLSET=msvc"
    call "%FOUND_PATH%VCVARS32.BAT"
    set "BOOST_JAM_TOOLSET_ROOT=%MSVCDir%\"
    exit /b 0)
if EXIST "C:\Borland\BCC55\Bin\bcc32.exe" (
    set "BOOST_JAM_TOOLSET=borland"
    set "BOOST_JAM_TOOLSET_ROOT=C:\Borland\BCC55\"
    exit /b 0)
call :Test_Path bcc32.exe
if not errorlevel 1 (
    set "BOOST_JAM_TOOLSET=borland"
    set "BOOST_JAM_TOOLSET_ROOT=%FOUND_PATH%..\"
    exit /b 0)
call :Test_Path icl.exe
if not errorlevel 1 (
    set "BOOST_JAM_TOOLSET=intel-win32"
    set "BOOST_JAM_TOOLSET_ROOT=%FOUND_PATH%..\"
    exit /b 0)
if EXIST "C:\MinGW\bin\gcc.exe" (
    set "BOOST_JAM_TOOLSET=mingw"
    set "BOOST_JAM_TOOLSET_ROOT=C:\MinGW\"
    exit /b 0)
REM Could not find a suitable toolset
exit /b 1
