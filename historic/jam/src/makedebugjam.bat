set VISUALC=c:\tools\msvc6\vc98
set JAM_TOOLSET=VISUALC
rm -rf bin.ntx86
mkdir bin.ntx86
nmake -fbuilds/win32-visualc.mk JAMBASE= BOOST_ROOT= BOOST_BUILD_PATH= CFLAGS="/GZ /Zi /MLd -DNT" CCFLAGS="/GZ /Zi /MLd" LINKLIBS="c:\tools\msvc6\vc98\lib\advapi32.lib c:\tools\msvc6\vc98\lib\oldnames.lib c:\tools\msvc6\vc98\lib\gdi32.lib c:\tools\msvc6\vc98\lib\user32.lib c:\tools\msvc6\vc98\lib\kernel32.lib" LINKFLAGS="/DEBUG" YACC="bison -t -d -l -v --debug --yacc" YACCFILES="y.tab" %*
