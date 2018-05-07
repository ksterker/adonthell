dnl **************************************************************
dnl Process this file with autoconf to produce a configure script.
dnl **************************************************************

AC_PREREQ(2.59)
AC_INIT([Adonthell], [0.4.0])
AC_CONFIG_SRCDIR([README])
AC_CONFIG_AUX_DIR(.)

dnl Keep the correct libtool macros in-tree
AC_CONFIG_MACRO_DIR([m4])

dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])

dnl ********************
dnl Checks for programs.
dnl ********************

AC_PROG_LN_S
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_MAKE_SET

dnl *********
dnl Endianess
dnl *********

AC_C_BIGENDIAN(CXXFLAGS="$CXXFLAGS -D__BIG_ENDIAN__", :, :)

dnl ************************************
dnl set up Objective C compiler for OS X
dnl ************************************
_AM_DEPENDENCIES([OBJC])

OBJC="gcc"
AC_SUBST(OBJC)

if test x$ac_cv_cxx_compiler_gnu = xyes; then
   CXXFLAGS="$CXXFLAGS -std=c++0x -fno-exceptions -fno-strict-aliasing"
fi

case "$target" in
    *-apple-darwin*)
        osx="yes"
	LDFLAGS="$LDFLAGS -headerpad_max_install_names"
        ;;
    *-*-cygwin* | *-*-mingw32*)
        LDFLAGS="$LDFLAGS -no-undefined"
        ;;
esac

AM_CONDITIONAL(OSX_COMPILE, test x$osx = xyes)


dnl *******
dnl libtool
dnl *******

AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

AC_CHECK_LIB(ltdl, lt_dlinit,,echo "Adonthell requires libltdl. Exitting...";exit 1)

dnl ********************
dnl Additional arguments
dnl ********************
AC_ARG_ENABLE(unix-install,
[  --disable-unix-install  Disable a Unix-like install (enabled by default)],
            unixinst=$enableval, unixinst=yes)
AC_ARG_WITH(data-dir,
[  --with-data-dir=DIR     Directory where game data will be installed],
            gamedatadir="$withval", gamedatadir=none)
AC_ARG_WITH(py-libs,
[  --with-py-libs=LIBS     Override Python libraries auto-detection],
            pylibs="$withval", pylibs=none)
AC_ARG_WITH(py-cflags,
[  --with-py-cflags=FLAGS  Override Python cflags auto-detection],
            pycflags="$withval", pycflags=none)
AC_ARG_ENABLE(dev-build,
[  --enable-dev-build  Enable developer build with unit tests and debug symbols (disabled by default)],
            devbuild=$enableval, devbuild=no)

AC_ARG_VAR(GTEST_DIR, [path to Google Test sources (default /usr/src/gtest)])
AC_ARG_VAR(GMOCK_DIR, [path to Google Mock sources (default /usr/src/gmock)])

AC_CHECK_LIB(z, main,,echo "Adonthell requires Zlib. Exitting...";exit 1)

dnl ******************************
dnl Tell that we are using libtool
dnl ******************************
CXXFLAGS="$CXXFLAGS -DUSE_LIBTOOL"

dnl **********************
dnl Set the data directory
dnl **********************

if test x$gamedatadir = xnone; then
  if test x$unixinst = xno; then
    CXXFLAGS="$CXXFLAGS -DSINGLE_DIR_INST"
    case "$target" in
      *-*-darwin*)
        makebundle="yes"
        backenddir="../PlugIns"
        gamedatadir="../Resources"
      ;;
      *)
        gamedatadir="."
        backenddir="./lib/adonthell"
      ;;
    esac
  else
    case "$target" in
      *-*-cygwin* | *-*-mingw32*)
        gamedatadir="."
        backenddir="./lib/adonthell"
	LDFLAGS="$LDFLAGS -Wl,--export-all-symbols"
      ;;
      *-*-beos*)
        gamedatadir="/boot/home/config/share/adonthell"
      ;;
      *)
        backenddir="${libdir}/adonthell"
        gamedatadir="${datadir}/adonthell"
      ;;
    esac
  fi
fi

CXXFLAGS="$CXXFLAGS -DDATA_DIR=\"\\\"$gamedatadir\\\"\""
AC_SUBST(backenddir)

dnl ***************
dnl Find pkg-config
dnl ***************
AC_PATH_PROG(PKG_CONFIG, [pkg-config], [no])
if test x$PKG_CONFIG = xno ; then
    AC_MSG_ERROR([*** pkg-config not found. See http://www.freedesktop.org/software/pkgconfig/])
fi

if $PKG_CONFIG --atleast-pkgconfig-version 0.14 ; then
    :
else
    AC_MSG_ERROR([*** pkg-config too old; version 0.14 or better required.])
fi


dnl *****************
dnl Check for libxml2
dnl *****************
XML_VERSION=2.5.0
AM_PATH_XML2($XML_VERSION,
    :,
    AC_MSG_ERROR([*** libxml version >= $XML_VERSION not found!])
)


dnl *************
dnl Check for SDL
dnl *************

dnl try sdl >= 2.0.0 first
AM_PATH_SDL2([2.0.0],
    [
        sdlbackenddir=sdl2
        sdlmixer_prefix=SDL2
    ],
	[
    	unset ac_cv_path_SDL_CONFIG
        sdlmixer_prefix=SDL
        SDL_VERSION=1.2.0
        AM_PATH_SDL([$SDL_VERSION],
            [sdlbackenddir=sdl],
            [AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])]
        )]
)

dnl *******************
dnl Check for SDL-mixer
dnl *******************

AC_CHECK_HEADER([${sdlmixer_prefix}/SDL_mixer.h],,
	[AC_MSG_ERROR([*** Cannot find the ${sdlmixer_prefix}/SDL_mixer.h header
	goto http://www.libsdl.org to download or
	check with your distibution.]
	); exit])
AC_CHECK_LIB([${sdlmixer_prefix}_mixer],
	[Mix_OpenAudio],
	[:],
	[AC_MSG_ERROR([*** Cannot find the ${sdlmixer_prefix}_mixer library
	goto http://www.libsdl.org to download or
	check with your distibution.]
	); exit])
SDL_MIXER_LIBS=-l${sdlmixer_prefix}_mixer
AC_SUBST(SDL_MIXER_LIBS)

dnl *******************
dnl Check for libpng
dnl *******************
dnl TODO Need to check for library also
AC_CHECK_HEADER([png.h],,
	[AC_MSG_ERROR([*** Cannot find the png.h header
	goto http://www.libpng.org/ to download or
	check with your distibution.]
	); exit])

dnl ****************
dnl Check for Python
dnl ****************

AM_PATH_PYTHON([2.0])

	dnl convert windows paths to cygwin paths
	dnl required when building the python wrappers

case "$target" in
	*-*-cygwin* | *-*-mingw32*)
		pyexecdir=`cd $pyexecdir && pwd`
		pythondir=`cd $pythondir && pwd`
	;;
esac


   dnl Find the Python.h header file

if test "x$pycflags" = xnone; then
   AC_MSG_CHECKING(for Python header files)
   changequote(<<, >>)
   PYINCLUDE=`$PYTHON -c 'import sys ; print "%s/include/python%s" % (sys.prefix, sys.version[:3])'`
   changequote([, ])

   dnl convert windows paths to cygwin paths
   
    case "$target" in
    *-*-cygwin* | *-*-mingw32*)
        PYINCLUDE=`cd $PYINCLUDE && pwd`
        ;;
    esac
   
    if test -r "$PYINCLUDE/Python.h"; then
        PY_CFLAGS="-I$PYINCLUDE"
        AC_SUBST(PY_CFLAGS)
    else
        AC_MSG_ERROR([Could not find Python.h in $PYINCLUDE])
    fi
    AC_MSG_RESULT(found)
else
    PY_CFLAGS=${pycflags}
    AC_MSG_CHECKING(for Python cflags)
    AC_MSG_RESULT($PY_CFLAGS)
    AC_SUBST(PY_CFLAGS)
fi

   dnl Find the Python library

if test "x$pylibs" = xnone; then
   AC_MSG_CHECKING(for Python library)
   PYLIB=""
   changequote(<<, >>)
   PYPREFIX=`$PYTHON -c 'import sys; print sys.prefix'`
   PYLIBVER=`$PYTHON -c 'import sys; print sys.version[:3]'`
   changequote([, ])

   dnl look for a framework build of python first
   fw=`echo "$PYPREFIX" | sed 's/.*\(Python\.framework\).*/\1/;'`
   if test "x$fw" != x && test "x$osx" = "xyes"; then
       PY_LIBS="-framework Python"
       PY_FWDIR=`$PYTHON -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_lib()'`
       PY_SPDIR="$pyexecdir"
       AC_SUBST(PY_FWDIR)
       AC_SUBST(PY_SPDIR)
       AC_MSG_RESULT(found)
   else
       py_paths="$PYPREFIX/lib64/python$PYLIBVER/config $PYPREFIX/lib64 \
    $PYPREFIX/lib/python$PYLIBVER/config-${target_cpu}-${target_os} \
	$PYPREFIX/lib/python$PYLIBVER/config $PYPREFIX/lib"

       dnl Try for specific version first, then the generic version, then panic

       for ppath in $py_paths ; do
           echo "searching in $ppath" >&AS_MESSAGE_LOG_FD
           if test -r "$ppath/libpython$PYLIBVER.so" -o -r "$ppath/libpython$PYLIBVER.a"; then
               PYLIB="-L$ppath -lpython$PYLIBVER"
               break
           fi
           if test -r "$ppath/libpython.so" -o -r "$ppath/libpython.a"; then
               PYLIB="-L$ppath -lpython"
               break
           fi
       done

       if test "x$PYLIB" != x ; then
           PY_LIBS="$PYLIB $PY_LIBS"
           AC_MSG_RESULT(found)
       else
           AC_MSG_ERROR([*** Python library not found])
       fi
   fi

   dnl Get the libraries that a static python library depends on
   AC_MSG_CHECKING(for Python's dependencies)

   py_extradeps=`$PYTHON -c 'import distutils.sysconfig; print distutils.sysconfig.get_config_var("LIBS")'`
   py_sysextradeps=`$PYTHON -c 'import distutils.sysconfig; print distutils.sysconfig.get_config_var("SYSLIBS")'`

   PY_DEPS="$py_extradeps $py_sysextradeps"

   AC_MSG_RESULT($PY_DEPS)

    dnl only GNU ld seems to know -E flag
    
    if $LD -v 2>&1 </dev/null | egrep '(GNU|with BFD)' 1>&5; then
        PY_LIBS="-Wl,-E $PY_LIBS $PY_DEPS"
    else
        PY_LIBS="$PY_LIBS $PY_DEPS"
    fi
    AC_SUBST(PY_LIBS)
else
    PY_LIBS=${pylibs}
    AC_MSG_CHECKING(for Python ld flags)
    AC_MSG_RESULT($PY_LIBS)
    AC_SUBST(PY_LIBS)
fi

AM_CONDITIONAL(PY_FRAMEWORK, test x$PY_SPDIR != x$PY_FWDIR)

dnl **************
dnl Check for SWIG
dnl **************
SWIG_VERSION=1.3.24
SWIG_PROG($SWIG_VERSION)
SWIG_ENABLE_CXX()

dnl *********************************************************************************
dnl If this is a developer build, turn on debug symbols and check for gtest and gmock
dnl *********************************************************************************

if test x$devbuild = xyes; then
    CXXFLAGS="$CXXFLAGS -g -Wall -pedantic"

    test "x$GTEST_DIR" = "x" && GTEST_DIR="/usr/src/gtest"
    test "x$GMOCK_DIR" = "x" && GMOCK_DIR="/usr/src/gmock"

    AC_CHECK_LIB(gtest_main, main,
        [AC_SUBST(libgtest_CFLAGS, "")
         AC_SUBST(libgtest_LIBS, "-lgtest")
         AM_CONDITIONAL(BUILD_GTEST, false)],
        [AC_CHECK_FILE($GTEST_DIR/src/gtest-all.cc,
            [AC_SUBST(GTEST_DIR)
             AC_SUBST(libgtest_CFLAGS, "-I$GTEST_DIR/include")
             AC_SUBST(libgtest_LIBS, '-L$(top_builddir)/src/ -lgtest')
             AM_CONDITIONAL(BUILD_GTEST, true)],
            [AC_MSG_FAILURE([Missing gtest library])])
        ])  
        
    AC_CHECK_LIB(gmock_main, main,
        [AC_SUBST(libgmock_CFLAGS, "")
         AC_SUBST(libgmock_LIBS, "-lgmock")
         AM_CONDITIONAL(BUILD_GMOCK, false)],
        [AC_CHECK_FILE($GMOCK_DIR/src/gmock-all.cc,
            [AC_SUBST(GMOCK_DIR)
             AC_SUBST(libgmock_CFLAGS, "-I$GMOCK_DIR/include")
             AC_SUBST(libgmock_LIBS, '-L$(top_builddir)/src/ -lgmock')
             AM_CONDITIONAL(BUILD_GMOCK, true)],
            [AC_MSG_FAILURE([Missing gmock library])])
        ])  
else
    AM_CONDITIONAL(BUILD_GTEST, false)
    AM_CONDITIONAL(BUILD_GMOCK, false)
fi

dnl *********************
dnl Check for google-glog
dnl *********************

PKG_CHECK_MODULES([libglog], [libglog >= 0.3.0], [
    AC_DEFINE([HAVE_GLOG_H], [1], [Define to 1 if you have google-glog])], [:])
AC_SUBST(libglog_LIBS)
AC_SUBST(libglog_CFLAGS)

dnl ************
dnl other checks
dnl ************

AC_CHECK_FUNCS(nanosleep,,)

AC_SUBST(CXXFLAGS)

PKG_CHECK_MODULES(FT2, freetype2 >= 7.0.1, [
	AC_SUBST(FT2_LIBS)
	AC_SUBST(FT2_CFLAGS)
	],
	AC_MSG_ERROR([Cannot find freetype])
)

AC_SUBST(CXXFLAGS)
AC_SUBST(sdlbackenddir)

AC_CONFIG_FILES([
adonthell.pc
Makefile
src/Makefile
src/main/Makefile
src/base/Makefile
src/event/Makefile
src/gfx/Makefile
src/input/Makefile
src/audio/Makefile
src/gui/Makefile
src/python/Makefile
src/rpg/Makefile
src/world/Makefile
src/py-runtime/Makefile
src/py-wrappers/Makefile
src/py-wrappers/adonthell/Makefile
doc/Makefile
test/Makefile
test/data/Makefile
])
AC_OUTPUT
