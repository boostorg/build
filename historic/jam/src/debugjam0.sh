#!/bin/sh
# This script can be used to recover from aborted builds in which Jam has
# removed some of its products (e.g. jamgramtab.h) upon failing some action
./yyacc jamgram.y jamgramtab.h jamgram.yy
export VISUALC=c:\tools\msvc6\vc98
export JAM_TOOLSET=VISUALC
jam0 -sCCFLAGS="/GZ /Zi /MLd" -sYACC="bison -t -d -l -v --yacc" -sLINKLIBS="c:\tools\msvc6\vc98\lib\advapi32.lib c:\tools\msvc6\vc98\lib\oldnames.lib c:\tools\msvc6\vc98\lib\gdi32.lib c:\tools\msvc6\vc98\lib\user32.lib c:\tools\msvc6\vc98\lib\kernel32.lib" -sLINKFLAGS="/DEBUG"
