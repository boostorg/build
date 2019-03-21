@ECHO OFF

REM ~ Copyright 2002-2007 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

setlocal
goto Start


:Set_Error
color 00
goto :eof


:Clear_Error
ver >nul
goto :eof


:Error_Print
REM Output an error message and set the errorlevel to indicate failure.
setlocal
ECHO ###
ECHO ### %1
ECHO ###
ECHO ### You can specify the toolset as the argument, i.e.:
ECHO ###     .\build.bat msvc
ECHO ###
ECHO ### Toolsets supported by this script are: borland, como, gcc,
ECHO ###     gcc-nocygwin, intel-win32, metrowerks, mingw,
ECHO ###     vc11, vc12, vc14, vc141
ECHO ###
ECHO ### If you have Visual Studio 2017 installed you will need either update
ECHO ### the Visual Studio 2017 installer or run from VS 2017 Command Prompt
ECHO ### as we where unable to detect your toolset installation.
ECHO ###
call :Set_Error
endlocal
goto :eof


:Test_Option
REM Tests whether the given string is in the form of an option: "--*"
call :Clear_Error
setlocal
set test=%1
if not defined test (
    call :Set_Error
    goto Test_Option_End
)
set test=###%test%###
set test=%test:"###=%
set test=%test:###"=%
set test=%test:###=%
if not "-" == "%test:~1,1%" call :Set_Error
:Test_Option_End
endlocal
goto :eof


:Test_Empty
REM Tests whether the given string is not empty
call :Clear_Error
setlocal
set test=%1
if not defined test (
    call :Clear_Error
    goto Test_Empty_End
)
set test=###%test%###
set test=%test:"###=%
set test=%test:###"=%
set test=%test:###=%
if not "" == "%test%" call :Set_Error
:Test_Empty_End
endlocal
goto :eof


:Guess_Toolset
set local
REM Try and guess the toolset to bootstrap the build with...
REM Sets BOOST_JAM_TOOLSET to the first found toolset.
REM May also set BOOST_JAM_TOOLSET_ROOT to the
REM location of the found toolset.

call :Clear_Error
call :Test_Empty "%ProgramFiles%"
if not errorlevel 1 set "ProgramFiles=C:\Program Files"

REM Visual studio is by default installed to %ProgramFiles% on 32-bit machines and
REM %ProgramFiles(x86)% on 64-bit machines. Making a common variable for both.
call :Clear_Error
call :Test_Empty "%ProgramFiles(x86)%"
if errorlevel 1 (
    set "VS_ProgramFiles=%ProgramFiles(x86)%"
) else (
    set "VS_ProgramFiles=%ProgramFiles%"
)

call guess_toolset.bat
if errorlevel 1 (
    call :Error_Print "Could not find a suitable toolset."
    goto :eof)

endlocal
goto :eof


:Start
set BOOST_JAM_TOOLSET=
set BOOST_JAM_ARGS=

REM If no arguments guess the toolset;
REM or if first argument is an option guess the toolset;
REM otherwise the argument is the toolset to use.
call :Clear_Error
call :Test_Empty %1
if not errorlevel 1 (
    call :Guess_Toolset
    if not errorlevel 1 ( goto Setup_Toolset ) else ( goto Finish )
)

call :Clear_Error
call :Test_Option %1
if not errorlevel 1 (
    call :Guess_Toolset
    if not errorlevel 1 ( goto Setup_Toolset ) else ( goto Finish )
)

call :Clear_Error
set BOOST_JAM_TOOLSET=%1
shift
goto Setup_Toolset


:Setup_Toolset
REM Setup the toolset command and options. This bit of code
REM needs to be flexible enough to handle both when
REM the toolset was guessed at and found, or when the toolset
REM was indicated in the command arguments.
REM NOTE: The strange multiple "if ?? == _toolset_" tests are that way
REM because in BAT variables are subsituted only once during a single
REM command. A complete "if ... else ..."
REM is a single command, even though it's in multiple lines here.
:Setup_Args
call :Clear_Error
call :Test_Empty %1
if not errorlevel 1 goto Config_Toolset
call :Clear_Error
call :Test_Option %1
if errorlevel 1 (
    set BOOST_JAM_ARGS=%BOOST_JAM_ARGS% %1
    shift
    goto Setup_Args
)
:Config_Toolset
call config_toolset.bat
if "_%_known_%_" == "__" (
    call :Error_Print "Unknown toolset: %BOOST_JAM_TOOLSET%"
)
if errorlevel 1 goto Finish

echo ###
echo ### Using '%BOOST_JAM_TOOLSET%' toolset.
echo ###

set BJAM_SOURCES=
set BJAM_SOURCES=%BJAM_SOURCES% builtins.c class.c
set BJAM_SOURCES=%BJAM_SOURCES% command.c compile.c constants.c cwd.c
set BJAM_SOURCES=%BJAM_SOURCES% debug.c debugger.c
set BJAM_SOURCES=%BJAM_SOURCES% execcmd.c execnt.c filent.c filesys.c frames.c function.c
set BJAM_SOURCES=%BJAM_SOURCES% glob.c hash.c hcache.c hdrmacro.c headers.c jam.c
set BJAM_SOURCES=%BJAM_SOURCES% jambase.c jamgram.c lists.c make.c make1.c md5.c mem.c modules.c
set BJAM_SOURCES=%BJAM_SOURCES% native.c object.c option.c output.c parse.c pathnt.c
set BJAM_SOURCES=%BJAM_SOURCES% pathsys.c regexp.c rules.c scan.c search.c strings.c
set BJAM_SOURCES=%BJAM_SOURCES% subst.c timestamp.c variable.c w32_getreg.c
set BJAM_SOURCES=%BJAM_SOURCES% modules/order.c
set BJAM_SOURCES=%BJAM_SOURCES% modules/path.c
set BJAM_SOURCES=%BJAM_SOURCES% modules/property-set.c
set BJAM_SOURCES=%BJAM_SOURCES% modules/regex.c
set BJAM_SOURCES=%BJAM_SOURCES% modules/sequence.c
set BJAM_SOURCES=%BJAM_SOURCES% modules/set.c

@echo ON
%BOOST_JAM_CXX% %BOOST_JAM_OPT_JAM% %BJAM_SOURCES% %BOOST_JAM_OPT_LINK%
dir *.exe
copy /b .\b2.exe .\bjam.exe

:Finish
@exit /b %ERRORLEVEL%
