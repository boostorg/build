@ECHO OFF
REM Copyrigt (C) 2002 Rene Rivera.
REM Permission to copy, use, modify, sell and distribute this software
REM is granted provided this copyright notice appears in all copies.
REM This software is provided "as is" without express or implied
REM warranty, and with no claim as to its suitability for any purpose.

REM Reset the toolset.
set BOOST_JAM_TOOLSET=

REM The one option we support in the invocation
REM is the name of the toolset to force building
REM with.
if "x%1x" == "xx" goto Guess_Toolset
if "%1" == "metrowerks" goto Set_Metrowerks
if "%1" == "msvc" goto Set_MSVC_Defaut
if "%1" == "vc7" goto Set_MSVC7_Defaut
if "%1" == "borland" goto Set_Borland
ECHO ###
ECHO ### Unknown toolset: %1
ECHO ###
ECHO ### You can specify the toolset as the argument, i.e.:
ECHO ###     .\build.bat msvc
ECHO ###
goto Finish

REM Try and guess the toolset to bootstrap the build with...
:Guess_Toolset
if NOT "x%CWFolder%x" == "xx" goto Set_Metrowerks
if NOT "x%MSVCDir%x" == "xx" goto Set_MSVC
if EXIST "C:\Program Files\Microsoft Visual C++\VC98\bin\VCVARS32.BAT" goto Set_MSVC_Defaut
if EXIST "C:\Program Files\Microsoft Visual Studio\VC98\bin\VCVARS32.BAT" goto Set_MSVS_Defaut
if EXIST "C:\Program Files\Microsoft Visual Studio .NET\VC7\bin\VCVARS32.BAT" goto Set_MSVC7_Defaut
if EXIST "C:\Borland\BCC55\Bin\bcc32.exe" goto Set_Borland
ECHO "###"
ECHO "### Could not find a suitable toolset."
ECHO "###"
ECHO "### You can specify the toolset as the argument, i.e.:"
ECHO "###     .\build.bat msvc"
ECHO "###"
goto Finish

:Set_Metrowerks
set BOOST_JAM_TOOLSET=metrowerks
set BOOST_JAM_CC=mwcc -runtime staticsingle -O -DNT
set BOOST_JAM_OPT_JAM=-o bootstrap.%BOOST_JAM_TOOLSET%\jam0.exe
set BOOST_JAM_OPT_MKJAMBASE=-o bootstrap.%BOOST_JAM_TOOLSET%\mkjambase0.exe
goto Build

:Set_MSVC
if EXIST "%MSVCDir%\VC7\bin" (set BOOST_JAM_TOOLSET=vc7) else (set BOOST_JAM_TOOLSET=msvc)
set BOOST_JAM_CC=cl /nologo /GZ /Zi /MLd -DNT -DYYDEBUG %MSVCDIR%\lib\kernel32.lib
set BOOST_JAM_OPT_JAM=/Febootstrap.%BOOST_JAM_TOOLSET%\jam0
set BOOST_JAM_OPT_MKJAMBASE=/Febootstrap.%BOOST_JAM_TOOLSET%\mkjambase0
goto Build

:Set_MSVC_Default
call "C:\Program Files\Microsoft Visual C++\VC98\bin\VCVARS32.BAT"
goto Set_MSVC

:Set_MSVS_Default
call "C:\Program Files\Microsoft Visual Studio\VC98\bin\VCVARS32.BAT"
goto Set_MSVC

:Set_MSVC7_Default
call "C:\Program Files\Microsoft Visual Studio .NET\VC7\bin\VCVARS32.BAT"
goto Set_MSVC

:Set_Borland
set BOOST_JAM_TOOLSET=borland
set PATH=%PATH%;C:\Borland\BCC55\Bin
set BOOST_JAM_CC=bcc32 -WC -w- -q -IC:\Borland\BCC55\Include -LC:\Borland\BCC55\Lib -O2 /DNT -nbootstrap.%BOOST_JAM_TOOLSET%
set BOOST_JAM_OPT_JAM=-ejam0
set BOOST_JAM_OPT_MKJAMBASE=-emkjambasejam0
goto Build

:Build
ECHO "###"
ECHO "### Using '%BOOST_JAM_TOOLSET%' toolset."
ECHO "###"

set MKJAMBASE_SOURCES=mkjambase.c
set BJAM_SOURCES=
set BJAM_SOURCES=%BJAM_SOURCES% command.c compile.c execnt.c execunix.c execvms.c expand.c
set BJAM_SOURCES=%BJAM_SOURCES% filent.c fileos2.c fileunix.c filevms.c glob.c hash.c
set BJAM_SOURCES=%BJAM_SOURCES% hdrmacro.c headers.c jam.c jambase.c jamgram.c lists.c make.c make1.c
set BJAM_SOURCES=%BJAM_SOURCES% newstr.c option.c parse.c pathunix.c pathvms.c regexp.c
set BJAM_SOURCES=%BJAM_SOURCES% rules.c scan.c search.c subst.c timestamp.c variable.c modules.c
set BJAM_SOURCES=%BJAM_SOURCES% strings.c filesys.c builtins.c pwd.c

REM No "yyacc" script available, yet.
@ECHO ON
rd /S /Q bootstrap.%BOOST_JAM_TOOLSET%
md bootstrap.%BOOST_JAM_TOOLSET%
@if EXIST jambase.c goto Build_BJAM
%JAM_CC% %BOOST_JAM_MKJAMBASE_OPT%
@if NOT EXIST ".\bootstrap.%BOOST_JAM_TOOLSET%\mkjambase0.exe" goto Finish
".\bootstrap.%BOOST_JAM_TOOLSET%\mkjambase0.exe" jambase.c Jambase
:Build_BJAM
%BOOST_JAM_CC% %BOOST_JAM_OPT_JAM% %BJAM_SOURCES%
@if NOT EXIST ".\bootstrap.%BOOST_JAM_TOOLSET%\jam0.exe" goto Finish
.\bootstrap.%BOOST_JAM_TOOLSET%\jam0 -f build.jam -sBOOST_JAM_TOOLSET=%BOOST_JAM_TOOLSET%

:Finish
