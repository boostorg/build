@ECHO OFF
REM Copyrigt (C) 2002 Rene Rivera.
REM Permission to copy, use, modify, sell and distribute this software
REM is granted provided this copyright notice appears in all copies.
REM This software is provided "as is" without express or implied
REM warranty, and with no claim as to its suitability for any purpose.
setlocal
goto Start

:Error_Print
setlocal
ECHO ###
ECHO ### %1
ECHO ###
ECHO ### You can specify the toolset as the argument, i.e.:
ECHO ###     .\build.bat msvc
ECHO ###
ECHO ### Toolsets supported by this script are: borland, metrowerks, msvc, vc7
ECHO ###
set _error_=
endlocal
goto :eof

:Test_Path
setlocal & endlocal
setlocal
set test=%~$PATH:1
endlocal
if not errorlevel 1 set FOUND_PATH=%~dp$PATH:1
goto :eof

:Test_Option
setlocal & endlocal
setlocal
set test=%1
if not "-" == "%test:~0,1%" set _error_=
endlocal
goto :eof

REM Try and guess the toolset to bootstrap the build with...
:Guess_Toolset
setlocal & endlocal
if NOT "_%CWFolder%_" == "__" (set BOOST_JAM_TOOLSET=metrowerks& goto :eof)
setlocal & endlocal
call :Test_Path mwcc.exe
if not errorlevel 1 (set BOOST_JAM_TOOLSET=metrowerks& goto :eof)
setlocal & endlocal
if NOT "_%MSVCDir%_" == "__" (set BOOST_JAM_TOOLSET=msvc& goto :eof)
setlocal & endlocal
if EXIST "C:\Program Files\Microsoft Visual Studio\VC98\bin\VCVARS32.BAT" (set BOOST_JAM_TOOLSET=msvc& goto :eof)
setlocal & endlocal
if EXIST "C:\Program Files\Microsoft Visual C++\VC98\bin\VCVARS32.BAT" (set BOOST_JAM_TOOLSET=msvc& goto :eof)
setlocal & endlocal
if EXIST "C:\Program Files\Microsoft Visual Studio .NET\VC7\bin\VCVARS32.BAT" (set BOOST_JAM_TOOLSET=vc7& goto :eof)
setlocal & endlocal
call :Test_Path cl.exe
if not errorlevel 1 (set BOOST_JAM_TOOLSET=msvc& goto :eof)
setlocal & endlocal
if EXIST "C:\Borland\BCC55\Bin\bcc32.exe" (set BOOST_JAM_TOOLSET=borland& goto :eof)
setlocal & endlocal
call :Test_Path bcc32.exe
if not errorlevel 1 (set BOOST_JAM_TOOLSET=borland& goto :eof)
setlocal & endlocal
call :Error_Print "Could not find a suitable toolset."
goto :eof

:Start
REM Reset the toolset.
set BOOST_JAM_TOOLSET=

if "_%1_" == "__" (
    call :Guess_Toolset
) else (
    call :Test_Option %1
    if not errorlevel 1 (
        call :Guess_Toolset
    ) else (
        setlocal & endlocal
        set BOOST_JAM_TOOLSET=%1
        shift
    )
)
if errorlevel 1 goto Finish

if "_%BOOST_JAM_TOOLSET%_" == "_metrowerks_" (
    set BOOST_JAM_CC=mwcc -runtime staticsingle -DNT
    set BOOST_JAM_OPT_JAM=-o bootstrap.%BOOST_JAM_TOOLSET%\jam0.exe
    set BOOST_JAM_OPT_MKJAMBASE=-o bootstrap.%BOOST_JAM_TOOLSET%\mkjambase0.exe
    set _known_=1
)
if "_%BOOST_JAM_TOOLSET%_" == "_msvc_" (
    if EXIST "C:\Program Files\Microsoft Visual Studio\VC98\bin\VCVARS32.BAT" (
        call "C:\Program Files\Microsoft Visual Studio\VC98\bin\VCVARS32.BAT"
    ) else if EXIST "C:\Program Files\Microsoft Visual C++\VC98\bin\VCVARS32.BAT" (
        call "C:\Program Files\Microsoft Visual C++\VC98\bin\VCVARS32.BAT"
    )
)
if "_%BOOST_JAM_TOOLSET%_" == "_msvc_" (
    set BOOST_JAM_CC=cl /nologo /GZ /Zi /MLd -DNT -DYYDEBUG %MSVCDIR%\lib\kernel32.lib
    set BOOST_JAM_OPT_JAM=/Febootstrap.%BOOST_JAM_TOOLSET%\jam0
    set BOOST_JAM_OPT_MKJAMBASE=/Febootstrap.%BOOST_JAM_TOOLSET%\mkjambase0
    set _known_=1
)
if "_%BOOST_JAM_TOOLSET%_" == "_vc7_" (
    if EXIST "C:\Program Files\Microsoft Visual Studio .NET\VC7\bin\VCVARS32.BAT" (
        call "C:\Program Files\Microsoft Visual Studio .NET\VC7\bin\VCVARS32.BAT"
    )
)
if "_%BOOST_JAM_TOOLSET%_" == "_vc7_" (
    set BOOST_JAM_CC=cl /nologo /GZ /Zi /MLd -DNT -DYYDEBUG %MSVCDIR%\lib\kernel32.lib
    set BOOST_JAM_OPT_JAM=/Febootstrap.%BOOST_JAM_TOOLSET%\jam0
    set BOOST_JAM_OPT_MKJAMBASE=/Febootstrap.%BOOST_JAM_TOOLSET%\mkjambase0
    set _known_=1
)
if "_%BOOST_JAM_TOOLSET%_" == "_borland_" (
    call :Test_Path bcc32.exe
    if errorlevel 1 (
        if EXIST "C:\Borland\BCC55\Bin\bcc32.exe" (
            set PATH=%PATH%;C:\Borland\BCC55\Bin
            call :Test_Path bcc32.exe
        )
    )
)
if "_%BOOST_JAM_TOOLSET%_" == "_borland_" (
    if not errorlevel 1 (
        set BOOST_JAM_CC=bcc32 -WC -w- -q -I%FOUND_PATH%..\Include -L%FOUND_PATH%..\Lib /DNT -nbootstrap.%BOOST_JAM_TOOLSET%
        set BOOST_JAM_OPT_JAM=-ejam0
        set BOOST_JAM_OPT_MKJAMBASE=-emkjambasejam0
        set _known_=1
    )
)
if "_%_known_%_" == "__" (
    call :Error_Print "Unknown toolset: %BOOST_JAM_TOOLSET%"
)
if errorlevel 1 goto Finish

echo ###
echo ### Using '%BOOST_JAM_TOOLSET%' toolset.
echo ###

set MKJAMBASE_SOURCES=mkjambase.c
set BJAM_SOURCES=
set BJAM_SOURCES=%BJAM_SOURCES% command.c compile.c execnt.c execunix.c execvms.c expand.c
set BJAM_SOURCES=%BJAM_SOURCES% filent.c fileos2.c fileunix.c filevms.c glob.c hash.c
set BJAM_SOURCES=%BJAM_SOURCES% hdrmacro.c headers.c jam.c jambase.c jamgram.c lists.c make.c make1.c
set BJAM_SOURCES=%BJAM_SOURCES% newstr.c option.c parse.c pathunix.c pathvms.c regexp.c
set BJAM_SOURCES=%BJAM_SOURCES% rules.c scan.c search.c subst.c timestamp.c variable.c modules.c
set BJAM_SOURCES=%BJAM_SOURCES% strings.c filesys.c builtins.c pwd.c

REM ~ REM No "yyacc" script available, yet, and therefore no grammar bootstrap.
@echo ON
rd /S /Q bootstrap.%BOOST_JAM_TOOLSET%
md bootstrap.%BOOST_JAM_TOOLSET%
@if exist jambase.c goto Skip_Jambase
%JAM_CC% %BOOST_JAM_MKJAMBASE_OPT%
@if not exist ".\bootstrap.%BOOST_JAM_TOOLSET%\mkjambase0.exe" goto Skip_Jambase
".\bootstrap.%BOOST_JAM_TOOLSET%\mkjambase0.exe" jambase.c Jambase
:Skip_Jambase
%BOOST_JAM_CC% %BOOST_JAM_OPT_JAM% %BJAM_SOURCES%
@if not exist ".\bootstrap.%BOOST_JAM_TOOLSET%\jam0.exe" goto Skip_Jam
.\bootstrap.%BOOST_JAM_TOOLSET%\jam0 -f build.jam --toolset=%BOOST_JAM_TOOLSET% %1 %2 %3 %4 %5 %6 %7 %8 %9
:Skip_Jam

:Finish
