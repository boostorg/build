@ECHO OFF

REM ~ Copyright 2002-2018 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

:Start
REM Setup the toolset command and options.
if "_%BOOST_JAM_TOOLSET%_" == "_msvc_" call :Config_MSVC
if "_%BOOST_JAM_TOOLSET%_" == "_vc11_" call :Config_VC11
if "_%BOOST_JAM_TOOLSET%_" == "_vc12_" call :Config_VC12
if "_%BOOST_JAM_TOOLSET%_" == "_vc14_" call :Config_VC14
if "_%BOOST_JAM_TOOLSET%_" == "_vc141_" call :Config_VC141
if "_%BOOST_JAM_TOOLSET%_" == "_borland_" call :Config_BORLAND
if "_%BOOST_JAM_TOOLSET%_" == "_como_" call :Config_COMO
if "_%BOOST_JAM_TOOLSET%_" == "_gcc_" call :Config_GCC
if "_%BOOST_JAM_TOOLSET%_" == "_gcc-nocygwin_" call :Config_GCC_NOCYGWIN
if "_%BOOST_JAM_TOOLSET%_" == "_intel-win32_" call :Config_INTEL_WIN32
if "_%BOOST_JAM_TOOLSET%_" == "_mingw_" call :Config_MINGW
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DNT"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DNDEBUG"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DOPT_HEADER_CACHE_EXT"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DOPT_GRAPH_DEBUG_EXT"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DOPT_SEMAPHORE"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DOPT_AT_FILES"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DOPT_DEBUG_PROFILE"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DJAM_DEBUGGER"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DOPT_FIX_TARGET_VARIABLES_EXT"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DOPT_IMPROVED_PATIENCE_EXT"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DYYSTACKSIZE=5000"
set "BOOST_JAM_OPT_JAM=%BOOST_JAM_OPT_JAM% -DYYINITDEPTH=5000"
exit /b %errorlevel%

:Call_If_Exists
if EXIST %1 call %*
goto :eof

:Config_MSVC
if NOT "_%MSVCDir%_" == "__" (
    set "BOOST_JAM_TOOLSET_ROOT=%MSVCDir%\"
    )
call :Call_If_Exists "%BOOST_JAM_TOOLSET_ROOT%bin\VCVARS32.BAT"
if not "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    set "PATH=%BOOST_JAM_TOOLSET_ROOT%bin;%PATH%"
    )
set "BOOST_JAM_CXX=cl /nologo /Zi /MT /TP /Feb2 /wd4996 /Ox /GL"
set "BOOST_JAM_OPT_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC11
if NOT "_%VS110COMNTOOLS%_" == "__" (
    set "BOOST_JAM_TOOLSET_ROOT=%VS110COMNTOOLS%..\..\VC\"
    )
if "_%VCINSTALLDIR%_" == "__" call :Call_If_Exists "%BOOST_JAM_TOOLSET_ROOT%VCVARSALL.BAT" %BOOST_JAM_ARGS%
if NOT "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    if "_%VCINSTALLDIR%_" == "__" (
        set "PATH=%BOOST_JAM_TOOLSET_ROOT%bin;%PATH%"
        ) )
set "BOOST_JAM_CXX=cl /nologo /Zi /MT /TP /Feb2 /wd4996 /Ox /GL"
set "BOOST_JAM_OPT_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC12
if NOT "_%VS120COMNTOOLS%_" == "__" (
    set "BOOST_JAM_TOOLSET_ROOT=%VS120COMNTOOLS%..\..\VC\"
    )

if "_%BOOST_JAM_ARCH%_" == "__" set BOOST_JAM_ARCH=x86
set BOOST_JAM_ARGS=%BOOST_JAM_ARGS% %BOOST_JAM_ARCH%

if "_%VCINSTALLDIR%_" == "__" call :Call_If_Exists "%BOOST_JAM_TOOLSET_ROOT%VCVARSALL.BAT" %BOOST_JAM_ARGS%
if NOT "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    if "_%VCINSTALLDIR%_" == "__" (
        set "PATH=%BOOST_JAM_TOOLSET_ROOT%bin;%PATH%"
        ) )
set "BOOST_JAM_CXX=cl /nologo /Zi /MT /TP /Feb2 /wd4996 /Ox /GL"
set "BOOST_JAM_OPT_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC14
if "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    if NOT "_%VS140COMNTOOLS%_" == "__" (
        set "BOOST_JAM_TOOLSET_ROOT=%VS140COMNTOOLS%..\..\VC\"
    ))

if "_%BOOST_JAM_ARCH%_" == "__" set BOOST_JAM_ARCH=x86
set BOOST_JAM_ARGS=%BOOST_JAM_ARGS% %BOOST_JAM_ARCH%

if "_%VCINSTALLDIR%_" == "__" call :Call_If_Exists "%BOOST_JAM_TOOLSET_ROOT%VCVARSALL.BAT" %BOOST_JAM_ARGS%
if NOT "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    if "_%VCINSTALLDIR%_" == "__" (
        set "PATH=%BOOST_JAM_TOOLSET_ROOT%bin;%PATH%"
        ) )
set "BOOST_JAM_CXX=cl /nologo /Zi /MT /TP /Feb2 /wd4996 /Ox /GL"
set "BOOST_JAM_OPT_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC141
call vswhere_usability_wrapper.cmd
REM Reset ERRORLEVEL since from now on it's all based on ENV vars
ver > nul 2> nul
if "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    if NOT "_%VS150COMNTOOLS%_" == "__" (
        set "BOOST_JAM_TOOLSET_ROOT=%VS150COMNTOOLS%..\..\VC\"
    ))

if "_%BOOST_JAM_ARCH%_" == "__" set BOOST_JAM_ARCH=x86
set BOOST_JAM_ARGS=%BOOST_JAM_ARGS% %BOOST_JAM_ARCH%

REM return to current directory as vsdevcmd_end.bat switches to %USERPROFILE%\Source if it exists.
pushd %CD%
if "_%VSINSTALLDIR%_" == "__" call :Call_If_Exists "%BOOST_JAM_TOOLSET_ROOT%Auxiliary\Build\vcvarsall.bat" %BOOST_JAM_ARGS%
popd
set "BOOST_JAM_CXX=cl /nologo /Zi /MT /TP /Feb2 /wd4996 /Ox /GL"
set "BOOST_JAM_OPT_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_BORLAND
if "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    call :Test_Path bcc32.exe )
if "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    if not errorlevel 1 (
        set "BOOST_JAM_TOOLSET_ROOT=%FOUND_PATH%..\"
        ) )
if not "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    set "PATH=%BOOST_JAM_TOOLSET_ROOT%Bin;%PATH%"
    )
set "BOOST_JAM_CXX=bcc32 -WC -w- -q -I%BOOST_JAM_TOOLSET_ROOT%Include -L%BOOST_JAM_TOOLSET_ROOT%Lib -nbootstrap"
set "BOOST_JAM_OPT_JAM=-eb2"
set "_known_=1"
goto :eof

:Config_COMO
set "BOOST_JAM_CXX=como"
set "BOOST_JAM_OPT_JAM=-o b2.exe"
set "_known_=1"
goto :eof

:Config_GCC
set "BOOST_JAM_CXX=gcc"
set "BOOST_JAM_OPT_JAM=-o b2.exe"
set "_known_=1"
goto :eof

:Config_GCC_NOCYGWIN
set "BOOST_JAM_CXX=gcc -mno-cygwin"
set "BOOST_JAM_OPT_JAM=-o b2.exe"
set "_known_=1"
goto :eof

:Config_INTEL_WIN32
set "BOOST_JAM_CXX=icl /nologo"
set "BOOST_JAM_OPT_JAM=/Feb2"
set "_known_=1"
goto :eof

:Config_MINGW
if not "_%BOOST_JAM_TOOLSET_ROOT%_" == "__" (
    set "PATH=%BOOST_JAM_TOOLSET_ROOT%bin;%PATH%"
    )
set "BOOST_JAM_CXX=gcc"
set "BOOST_JAM_OPT_JAM=-o b2.exe"
set "_known_=1"
goto :eof
