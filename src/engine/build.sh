#!/bin/sh

#~ Copyright 2002-2018 Rene Rivera.
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or copy at
#~ http://www.boost.org/LICENSE_1_0.txt)

# Reset the toolset.
BOOST_JAM_TOOLSET=
BOOST_JAM_OS=

# Run a command, and echo before doing so. Also checks the exit status and quits
# if there was an error.
echo_run ()
{
    echo "$@"
    $@
    r=$?
    if test $r -ne 0 ; then
        exit $r
    fi
}

# Print an error message, and exit with a status of 1.
error_exit ()
{
    echo "###"
    echo "###" "$@"
    echo "###"
    echo "### You can specify the toolset as the argument, i.e.:"
    echo "###     ./build.sh gcc"
    echo "###"
    echo "### Toolsets supported by this script are:"
    echo "###     acc, clang, como, gcc, intel-darwin, intel-linux,"
    echo "###     kcc, kylix, mipspro, pathscale, pgi, qcc, sun, sunpro,"
    echo "###     tru64cxx, vacpp"
    echo "###"
    echo "### A special toolset; cc, is available which is used as a fallback"
    echo "### when a more specific toolset is not found and the cc command is"
    echo "### detected. The 'cxx' toolset will use the CXX, CXXFLAGS, and LIBS"
    echo "### environment variables, if present."
    echo "###"
    echo "### Similarly, the cross-cc toolset is available for cross-compiling"
    echo "### by using the BUILD_CC, BUILD_CFLAGS, and BUILD_LDFLAGS environment"
    echo "### variables to compile binaries that will be executed on the build"
    echo "### system. This allows CC etc. to be set for cross-compilers to be"
    echo "### propagated to subprocesses."
    echo "###"
    exit 1
}

# Check that a command is in the PATH.
test_path ()
{
    if `command -v command 1>/dev/null 2>/dev/null`; then
        command -v $1 1>/dev/null 2>/dev/null
    else
        hash $1 1>/dev/null 2>/dev/null
    fi
}

# Check that the OS name, as returned by "uname", is as given.
test_uname ()
{
    if test_path uname; then
        test `uname` = $*
    fi
}

# Try and guess the toolset to bootstrap the build with...
guess_toolset ()
{
    if test_uname Darwin ; then BOOST_JAM_TOOLSET=clang
    elif test_uname IRIX ; then BOOST_JAM_TOOLSET=mipspro
    elif test_uname IRIX64 ; then BOOST_JAM_TOOLSET=mipspro
    elif test_uname OSF1 ; then BOOST_JAM_TOOLSET=tru64cxx
    elif test_uname QNX && test_path qcc ; then BOOST_JAM_TOOLSET=qcc
    elif test_uname Linux && test_path xlc; then
       if /usr/bin/lscpu | grep Byte | grep Little > /dev/null 2>&1 ; then
          # Little endian linux
          BOOST_JAM_TOOLSET=xlcpp
       else
          #Big endian linux
          BOOST_JAM_TOOLSET=vacpp
       fi
    elif test_uname AIX && test_path xlc; then BOOST_JAM_TOOLSET=vacpp
    elif test_uname FreeBSD && test_path freebsd-version && test_path clang; then BOOST_JAM_TOOLSET=clang
    elif test_path gcc ; then BOOST_JAM_TOOLSET=gcc
    elif test_path clang ; then BOOST_JAM_TOOLSET=clang
    elif test_path icc ; then BOOST_JAM_TOOLSET=intel-linux
    elif test -r /opt/intel/cc/9.0/bin/iccvars.sh ; then
        BOOST_JAM_TOOLSET=intel-linux
        BOOST_JAM_TOOLSET_ROOT=/opt/intel/cc/9.0
    elif test -r /opt/intel_cc_80/bin/iccvars.sh ; then
        BOOST_JAM_TOOLSET=intel-linux
        BOOST_JAM_TOOLSET_ROOT=/opt/intel_cc_80
    elif test -r /opt/intel/compiler70/ia32/bin/iccvars.sh ; then
        BOOST_JAM_TOOLSET=intel-linux
        BOOST_JAM_TOOLSET_ROOT=/opt/intel/compiler70/ia32/
    elif test -r /opt/intel/compiler60/ia32/bin/iccvars.sh ; then
        BOOST_JAM_TOOLSET=intel-linux
        BOOST_JAM_TOOLSET_ROOT=/opt/intel/compiler60/ia32/
    elif test -r /opt/intel/compiler50/ia32/bin/iccvars.sh ; then
        BOOST_JAM_TOOLSET=intel-linux
        BOOST_JAM_TOOLSET_ROOT=/opt/intel/compiler50/ia32/
    elif test_path pgcc ; then BOOST_JAM_TOOLSET=pgi
    elif test_path pathcc ; then BOOST_JAM_TOOLSET=pathscale
    elif test_path como ; then BOOST_JAM_TOOLSET=como
    elif test_path KCC ; then BOOST_JAM_TOOLSET=kcc
    elif test_path bc++ ; then BOOST_JAM_TOOLSET=kylix
    elif test_path aCC ; then BOOST_JAM_TOOLSET=acc
    elif test_uname HP-UX ; then BOOST_JAM_TOOLSET=acc
    elif test -r /opt/SUNWspro/bin/cc ; then
        BOOST_JAM_TOOLSET=sunpro
        BOOST_JAM_TOOLSET_ROOT=/opt/SUNWspro/
    # Test for "cxx" as the default fallback.
    elif test_path $CXX ; then BOOST_JAM_TOOLSET=cxx
    elif test_path cxx ; then
        BOOST_JAM_TOOLSET=cxx
        CXX=cxx
    elif test_path cpp ; then
        BOOST_JAM_TOOLSET=cxx
        CXX=cpp
    fi
    if test "$BOOST_JAM_TOOLSET" = "" ; then
        error_exit "Could not find a suitable toolset."
    fi
}

check_debug_build ()
{
    while test $# -gt 0
    do
        case "$1" in
            --debug) return 0 ;;
        esac
        shift
    done
    return 1
}

check_python_build ()
{
    while test $# -gt 0
    do
        case "$1" in
            --python) return 0 ;;
        esac
        shift
    done
    return 1
}

# The one option we support in the invocation
# is the name of the toolset to force building
# with.
case "$1" in
    --guess-toolset) guess_toolset ; echo "$BOOST_JAM_TOOLSET" ; exit 1 ;;
    -*) guess_toolset ;;
    ?*) BOOST_JAM_TOOLSET=$1 ; shift ;;
    *) guess_toolset ;;
esac
BOOST_JAM_OPT_JAM="\
 -DNDEBUG \
 -DOPT_HEADER_CACHE_EXT \
 -DOPT_GRAPH_DEBUG_EXT \
 -DOPT_SEMAPHORE \
 -DOPT_AT_FILES \
 -DOPT_DEBUG_PROFILE \
 -DJAM_DEBUGGER \
 -DOPT_FIX_TARGET_VARIABLES_EXT \
 -DOPT_IMPROVED_PATIENCE_EXT \
 -DYYSTACKSIZE=5000 \
 -DYYINITDEPTH=5000 \
 -o b2"
BOOST_JAM_OPT_MKJAMBASE="-o mkjambase"
BOOST_JAM_OPT_YYACC="-o yyacc"
case $BOOST_JAM_TOOLSET in

    gcc)
        echo_run ${CXX:=gcc} --version
        # Check whether it's MinGW GCC, which has Windows headers and none of POSIX ones.
        machine=$(${CXX} -dumpmachine 2>/dev/null)
        if [ $? -ne 0 ]; then
            echo "BOOST_JAM_TOOLSET is gcc, but the 'gcc' command cannot be executed."
            echo "Make sure 'gcc' is in PATH, or use a different toolset."
            exit 1
        fi
        case $machine in
        *mingw*)
        # MinGW insists that its bin directory be in PATH.
        if test -r ${BOOST_JAM_TOOLSET_ROOT}bin/gcc ; then
            export PATH=${BOOST_JAM_TOOLSET_ROOT}bin:$PATH
        fi
        BOOST_JAM_CXX="${CXX} -x c++ -DNT"
        BOOST_RELEASE="-O2 -s"
        BOOST_DEBUG="-O0 -g"
        BOOST_JAM_OS="NT"
        ;;

        *)
        BOOST_JAM_CXX="${CXX} -x c++ -std=c++11"
        BOOST_RELEASE="-O2 -s"
        BOOST_DEBUG="-O0 -g"
        BOOST_PYTHON="`python-config --includes --libs` -DHAVE_PYTHON -Wno-deprecated-register"
        esac
    ;;

    intel-darwin)
    BOOST_JAM_CXX="icc"
    BOOST_RELEASE="-O3 -s"
    BOOST_DEBUG="-O0 -g -p"
    ;;

    intel-linux)
    test_path icc >/dev/null 2>&1
    if test $? ; then
	BOOST_JAM_CXX=`test_path icc`
	echo "Found $BOOST_JAM_CXX in environment"
	BOOST_JAM_TOOLSET_ROOT=`echo $BOOST_JAM_CXX | sed -e 's/bin.*\/icc//'`
	# probably the most widespread
	ARCH=intel64
    else
	echo "No intel compiler in current path"
	echo "Look in a few old place for legacy reason"
	if test -r /opt/intel/cc/9.0/bin/iccvars.sh ; then
            BOOST_JAM_TOOLSET_ROOT=/opt/intel/cc/9.0/
	elif test -r /opt/intel_cc_80/bin/iccvars.sh ; then
            BOOST_JAM_TOOLSET_ROOT=/opt/intel_cc_80/
	elif test -r /opt/intel/compiler70/ia32/bin/iccvars.sh ; then
            BOOST_JAM_TOOLSET_ROOT=/opt/intel/compiler70/ia32/
	elif test -r /opt/intel/compiler60/ia32/bin/iccvars.sh ; then
            BOOST_JAM_TOOLSET_ROOT=/opt/intel/compiler60/ia32/
	elif test -r /opt/intel/compiler50/ia32/bin/iccvars.sh ; then
            BOOST_JAM_TOOLSET_ROOT=/opt/intel/compiler50/ia32/
	fi
    fi
    if test -r ${BOOST_JAM_TOOLSET_ROOT}bin/iccvars.sh ; then
        # iccvars does not change LD_RUN_PATH. We adjust LD_RUN_PATH here in
        # order not to have to rely on ld.so.conf knowing the icc library
        # directory. We do this before running iccvars.sh in order to allow a
        # user to add modifications to LD_RUN_PATH in iccvars.sh.
        if test -z "${LD_RUN_PATH}"; then
            LD_RUN_PATH="${BOOST_JAM_TOOLSET_ROOT}lib"
        else
            LD_RUN_PATH="${BOOST_JAM_TOOLSET_ROOT}lib:${LD_RUN_PATH}"
        fi
        export LD_RUN_PATH
        . ${BOOST_JAM_TOOLSET_ROOT}bin/iccvars.sh $ARCH
    fi
    if test -z "$BOOST_JAM_CXX" ; then
	BOOST_JAM_CXX=icc
    fi
    BOOST_JAM_CXX="${BOOST_JAM_CXX} -Xlinker"
    BOOST_RELEASE="-O3 -s"
    BOOST_DEBUG="-O0 -g -p"
    ;;

    vacpp)
    BOOST_JAM_CXX="xlc"
    BOOST_RELEASE="-O3 -s -qstrict -qinline"
    BOOST_DEBUG="-g -qNOOPTimize -qnoinline -pg"
    ;;

    xlcpp)
    BOOST_JAM_CXX="xlC"
    BOOST_RELEASE="-s -O3 -qstrict -qinline"
    BOOST_DEBUG="-g -qNOOPTimize -qnoinline -pg"
    ;;

    como)
    BOOST_JAM_CXX="como --c"
    BOOST_RELEASE="--inlining"
    BOOST_DEBUG="--no_inlining"
    ;;

    kcc)
    BOOST_JAM_CXX="KCC"
    BOOST_RELEASE="+K2 -s"
    BOOST_DEBUG="+K0 -g"
    ;;

    kylix)
    BOOST_JAM_CXX="bc++ -tC -q"
    BOOST_RELEASE="-O2 -vi -w-inl -s"
    BOOST_DEBUG="-Od -v -vi-"
    ;;

    mipspro)
    BOOST_JAM_CXX="cc"
    BOOST_RELEASE="-O3 -g0 \"-INLINE:none\" -s"
    BOOST_DEBUG="-O0 -INLINE -g"
    ;;

    pathscale)
    BOOST_JAM_CXX="pathcc -s -O3"
    BOOST_RELEASE="-O3 -s"
    BOOST_DEBUG="-g"
    ;;

    pgi)
    BOOST_JAM_CXX="pgcc -s -O3"
    BOOST_RELEASE="-O3 -s"
    BOOST_DEBUG="-g"
    ;;

    sun*)
    if test -z "${BOOST_JAM_TOOLSET_ROOT}" -a -r /opt/SUNWspro/bin/cc ; then
        BOOST_JAM_TOOLSET_ROOT=/opt/SUNWspro/
    fi
    if test -r "${BOOST_JAM_TOOLSET_ROOT}bin/cc" ; then
        PATH=${BOOST_JAM_TOOLSET_ROOT}bin:${PATH}
        export PATH
    fi
    BOOST_JAM_CXX="cpp"
    BOOST_RELEASE="-xO3 -s"
    BOOST_DEBUG="-g"
    ;;

    clang*)
    echo_run ${CXX:=clang} --version
    BOOST_JAM_CXX="${CXX} -x c++ -O3 -std=c++11"
    BOOST_JAM_TOOLSET=clang
    BOOST_RELEASE="-O3 -s -flto"
    BOOST_DEBUG="-O0 -fno-inline -g"
    BOOST_PYTHON="`python-config --includes --libs` -DHAVE_PYTHON -Wno-deprecated-register"
    ;;

    tru64cxx)
    BOOST_JAM_CXX="cc"
    BOOST_RELEASE="-O5 -inline speed -s"
    BOOST_DEBUG="-O0 -pg -g"
    ;;

    acc)
    BOOST_JAM_CXX="cc"
    BOOST_RELEASE="-O3 -Ae -s"
    BOOST_DEBUG="-g -pg"
    ;;

    cxx)
    if test -z "${CXX}" ; then CXX=cxx ; fi
    BOOST_JAM_CXX=${CXX}
    BOOST_JAM_OPT_JAM="$BOOST_JAM_OPT_JAM $CXXFLAGS $LIBS"
    BOOST_JAM_OPT_MKJAMBASE="$BOOST_JAM_OPT_MKJAMBASE $CXXFLAGS $LIBS"
    BOOST_JAM_OPT_YYACC="$BOOST_JAM_OPT_YYACC $CXXFLAGS $LIBS"
    ;;

    cross-cc)
    if test -z "$BUILD_CC" ; then BUILD_CC=cc ; fi
    BOOST_JAM_CC=$BUILD_CC
    BOOST_JAM_OPT_JAM="$BOOST_JAM_OPT_JAM $BUILD_CFLAGS $BUILD_LDFLAGS"
    BOOST_JAM_OPT_MKJAMBASE="$BOOST_JAM_OPT_MKJAMBASE $BUILD_CFLAGS $BUILD_LDFLAGS"
    BOOST_JAM_OPT_YYACC="$BOOST_JAM_OPT_YYACC $BUILD_CFLAGS $BUILD_LDFLAGS"
    BOOST_JAM_TOOLSET=cc
    ;;

    qcc)
    BOOST_JAM_CXX="qcc"
    BOOST_RELEASE="-O3 -Wc,-finline-functions"
    BOOST_DEBUG="O0 -Wc,-fno-inline -g"
    ;;

    *)
    error_exit "Unknown toolset: $BOOST_JAM_TOOLSET"
    ;;
esac

echo "###"
echo "### Using '$BOOST_JAM_TOOLSET' toolset."
echo "###"

YYACC_SOURCES="yyacc.c"
MKJAMBASE_SOURCES="mkjambase.c"
BJAM_SOURCES="\
 builtins.c \
 class.c \
 command.c \
 compile.c \
 constants.c \
 cwd.c \
 debug.c \
 debugger.c \
 execcmd.c \
 filesys.c \
 frames.c \
 function.c \
 glob.c\
 hash.c \
 hcache.c \
 hdrmacro.c \
 headers.c \
 jam.c \
 jambase.c \
 jamgram.c \
 lists.c \
 make.c \
 make1.c \
 md5.c \
 mem.c \
 modules.c \
 native.c \
 object.c \
 option.c \
 output.c \
 parse.c \
 pathsys.c \
 regexp.c \
 rules.c \
 scan.c \
 search.c \
 strings.c \
 subst.c \
 timestamp.c \
 variable.c \
 w32_getreg.c \
 modules/order.c \
 modules/path.c \
 modules/property-set.c \
 modules/regex.c \
 modules/sequence.c \
 modules/set.c \
 "
case $BOOST_JAM_OS in
    NT)
    BJAM_SOURCES="${BJAM_SOURCES} execnt.c filent.c pathnt.c"
    ;;

    *)
    BJAM_SOURCES="${BJAM_SOURCES} execunix.c fileunix.c pathunix.c"
    ;;
esac

if test ! -r jamgram.y -o ! -r jamgramtab.h ; then
    echo_run ${BOOST_JAM_CXX} ${BOOST_JAM_OPT_YYACC} ${YYACC_SOURCES}
    if test -x "./yyacc" ; then
        echo_run ./yyacc jamgram.y jamgramtab.h jamgram.yy
    fi
fi
if test ! -r jamgram.c -o ! -r jamgram.h ; then
    if test_path yacc ; then YACC="yacc -d"
    elif test_path bison ; then YACC="bison -y -d --yacc"
    fi
    echo_run $YACC jamgram.y
    mv -f y.tab.c jamgram.c
    mv -f y.tab.h jamgram.h
fi
if test ! -r jambase.c ; then
    echo_run ${BOOST_JAM_CXX} ${BOOST_JAM_OPT_MKJAMBASE} ${MKJAMBASE_SOURCES}
    if test -x "./mkjambase" ; then
        echo_run ./mkjambase jambase.c Jambase
    fi
fi
if check_debug_build "$@" ; then BOOST_JAM_CXX="${BOOST_JAM_CXX} ${BOOST_DEBUG}"
else BOOST_JAM_CXX="${BOOST_JAM_CXX} ${BOOST_RELEASE}"
fi
if check_python_build "$@" ; then BOOST_JAM_CXX="${BOOST_JAM_CXX} ${BOOST_PYTHON}"
fi
echo_run ${BOOST_JAM_CXX} ${BOOST_JAM_OPT_JAM} ${BJAM_SOURCES}
echo_run cp b2 bjam
