#!/bin/sh
./yyacc jamgram.y jamgramtab.h jamgram.yy
export VISUALC=c:\tools\msvc6\vc98
export JAM_TOOLSET=VISUALC
rm -rf bin.ntx86
nmake -fbuilds/win32-visualc.mk CFLAGS="/GZ /Zi /MLd -DNT"
CCFLAGS="/GZ /Zi /MLd" YACC="byacc" YACCFLAGS=-vd LINKLIBS="c:\tools\msvc6\vc98\lib\advapi32.lib c:\tools\msvc6\vc98\lib\oldnames.lib c:\tools\msvc6\vc98\lib\gdi32.lib c:\tools\msvc6\vc98\lib\user32.lib c:\tools\msvc6\vc98\lib\kernel32.lib" LINKFLAGS="/DEBUG"
