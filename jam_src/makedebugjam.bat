call c:\tools\msvc6\vc98\bin\vcvars32.bat
set VISUALC=c:\tools\msvc6\vc98
set JAM_TOOLSET=VISUALC
rm -rf bin.ntx86/jam.exe bin.ntx86/*.obj bin.ntx86/*.lib
set BOOST_ROOT=
set BOOST_BUILD_PATH=
set JAMBASE=
nmake -fbuilds/win32-visualc.mk JAMBASE= BOOST_ROOT= BOOST_BUILD_PATH= CFLAGS="/GZ /Zi /MLd -DNT -DYYDEBUG" CCFLAGS="/GZ /Zi /MLd -DYYDEBUG" LINKLIBS="c:\tools\msvc6\vc98\lib\advapi32.lib c:\tools\msvc6\vc98\lib\oldnames.lib c:\tools\msvc6\vc98\lib\gdi32.lib c:\tools\msvc6\vc98\lib\user32.lib c:\tools\msvc6\vc98\lib\kernel32.lib" LINKFLAGS="/DEBUG" YACC="byacc" YACCFILES="y.tab" YACCFLAGS=-vd %*
