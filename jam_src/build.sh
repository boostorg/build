#!/bin/sh
# Copyrigt (C) 2002 Rene Rivera.
# Permission to copy, use, modify, sell and distribute this software
# is granted provided this copyright notice appears in all copies.
# This software is provided "as is" without express or implied
# warranty, and with no claim as to its suitability for any purpose.

# Reset the toolset.
BOOST_JAM_TOOLSET=

function echo_run ()
{
    echo "$@"
    "$@"
    r=$?
    if test $r -ne 0 ; then
        exit $r
    fi
}
function error_exit()
{
    echo "###"
    echo "###" "$@"
    echo "###"
    echo "### You can specify the toolset as the argument, i.e.:"
    echo "###     ./build.sh gcc"
    echo "###"
    exit 1
}
function test_run ()
{
    q=`$* 2>/dev/null`
}
function test_uname ()
{
    if test_run uname; then
        test `uname` = $*
    fi
}
# Try and guess the toolset to bootstrap the build with...
function Guess_Toolset ()
{
    if test_uname Darwin ; then BOOST_JAM_TOOLSET=darwin
    elif test_run gcc --version ; then BOOST_JAM_TOOLSET=gcc
    elif [[ -e /opt/intel/compiler50/ia32/bin/iccvars.sh ]] ; then BOOST_JAM_TOOLSET=intel-linux
    elif test_run xlc ; then BOOST_JAM_TOOLSET=vacpp
    fi
    if test "$BOOST_JAM_TOOLSET" = "" ; then
        error_exit "Could not find a suitable toolset."
    fi
}

# The one option we support in the invocation
# is the name of the toolset to force building
# with.
if test "$1" = ""; then
    Guess_Toolset
else
    BOOST_JAM_TOOLSET=$1
fi
case $BOOST_JAM_TOOLSET in
    gcc)
    BOOST_JAM_CC=gcc
    BOOST_JAM_OPT_JAM="-O3 -o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-O3 -o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    darwin)
    BOOST_JAM_CC=cc
    BOOST_JAM_OPT_JAM="-O3 -o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-O3 -o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    intel-linux)
    echo_run /opt/intel/compiler50/ia32/bin/iccvars.sh
    BOOST_JAM_CC=icc
    BOOST_JAM_OPT_JAM="-Xlinker -O3 -o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-Xlinker -O3 -o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    vacpp)
    BOOST_JAM_CC=xlc
    BOOST_JAM_OPT_JAM="-qstrict -O3 -o bootstrap.$BOOST_JAM_TOOLSET/jam0"
    BOOST_JAM_OPT_MKJAMBASE="-qstrict -O3 -o bootstrap.$BOOST_JAM_TOOLSET/mkjambase0"
    ;;
    
    *)
    error_exit "Unknown toolset: $BOOST_JAM_TOOLSET"
    ;;
esac

echo "###"
echo "### Using '$BOOST_JAM_TOOLSET' toolset."
echo "###"

MKJAMBASE_SOURCES="mkjambase.c"
BJAM_SOURCES="\
 command.c compile.c execnt.c execunix.c execvms.c expand.c\
 filent.c fileos2.c fileunix.c filevms.c glob.c hash.c\
 hdrmacro.c headers.c jam.c jambase.c jamgram.c lists.c make.c make1.c\
 newstr.c option.c parse.c pathunix.c pathvms.c regexp.c\
 rules.c scan.c search.c subst.c timestamp.c variable.c modules.c\
 strings.c filesys.c builtins.c pwd.c"

echo_run rm -rf bootstrap.$BOOST_JAM_TOOLSET
echo_run mkdir bootstrap.$BOOST_JAM_TOOLSET
if [[ ! -a jamgram.y || ! -a jamgramtab.h ]] ; then
    echo_run ./yyacc jamgram.y jamgramtab.h jamgram.yy
fi
if [[ ! -a jambase.c ]] ; then
    echo_run ${BOOST_JAM_CC} ${BOOST_JAM_OPT_MKJAMBASE} ${MKJAMBASE_SOURCES}
    if test -e "./bootstrap.$BOOST_JAM_TOOLSET/mkjambase0" ; then
        echo_run ./bootstrap.$BOOST_JAM_TOOLSET/mkjambase0 jambase.c Jambase
    fi
fi
echo_run ${BOOST_JAM_CC} ${BOOST_JAM_OPT_JAM} ${BJAM_SOURCES}
if [[ -e "./bootstrap.$BOOST_JAM_TOOLSET/jam0" ]] ; then
    echo_run ./bootstrap.$BOOST_JAM_TOOLSET/jam0 -f build.jam -sBOOST_JAM_TOOLSET=$BOOST_JAM_TOOLSET
fi
