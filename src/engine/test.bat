SET cl_path_cmd="%~dp0..\tools\vc141helper\cl_path.cmd"
for /F "tokens=*" %%A IN ('cmd /D /S /C "%cl_path_cmd% 14.10"') DO if NOT "_%%A_" == "__" (
    if errorlevel 1 goto :toolset_error
    ECHO VC1 %%A
    set "BOOST_JAM_TOOLSET=vc1410"
    set "BOOST_JAM_TOOLSET_ROOT=%%A\VC\"
    goto :print)

if EXIST "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "BOOST_JAM_TOOLSET=vc1410"
    set "BOOST_JAM_TOOLSET_ROOT=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Enterprise\VC\"
    goto :print)

if EXIST "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "BOOST_JAM_TOOLSET=vc1410"
    set "BOOST_JAM_TOOLSET_ROOT=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Professional\VC\"
    goto :print)

if EXIST "%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "BOOST_JAM_TOOLSET=vc1410"
    set "BOOST_JAM_TOOLSET_ROOT=%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\"
    goto :print)

:toolset_error
ECHO "errorlevel %errorlevel%"
ECHO "Could not find a suitable toolset."

:print
ECHO BOOST_JAM_TOOLSET %BOOST_JAM_TOOLSET%
ECHO BOOST_JAM_TOOLSET_ROOT %BOOST_JAM_TOOLSET_ROOT%