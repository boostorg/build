set VISUALC=c:\tools\msvc6\vc98
set JAM_TOOLSET=VISUALC
rm -rf bin.ntx86
make -fbuilds/win32-gcc.mk CFLAGS="/GZ /Zi /MLd -DNT" CCFLAGS="/GZ /Zi /MLd" YACC="bison -t -d -l -v --yacc" LINKLIBS="c:\tools\msvc6\vc98\lib\advapi32.lib c:\tools\msvc6\vc98\lib\oldnames.lib c:\tools\msvc6\vc98\lib\gdi32.lib c:\tools\msvc6\vc98\lib\user32.lib c:\tools\msvc6\vc98\lib\kernel32.lib" LINKFLAGS="/DEBUG" %*
