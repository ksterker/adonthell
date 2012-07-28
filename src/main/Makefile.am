AM_CXXFLAGS = -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt sdl/linux.cc sdl/winnt.cc sdl/osx.h sdl/osx.m

adonthellincludedir = $(pkgincludedir)-@VERSION@/adonthell

## Our header files
pkgincludemaindir = $(adonthellincludedir)/main/
pkgincludemain_HEADERS = \
	adonthell.h

## Main library
lib_LTLIBRARIES = libadonthell_main.la

## Rules to build libmain
libadonthell_main_la_SOURCES = \
	adonthell.cc \
	main.cc

libadonthell_main_la_CXXFLAGS = $(PY_CFLAGS) -DPYTHONSPDIR=\"$(pyexecdir)\" $(AM_CXXFLAGS)
libadonthell_main_la_LIBADD = $(PY_LIBS) -lltdl \
    $(top_builddir)/src/base/libadonthell_base.la \
    $(top_builddir)/src/gfx/libadonthell_gfx.la \
    $(top_builddir)/src/audio/libadonthell_audio.la \
    $(top_builddir)/src/input/libadonthell_input.la \
    $(top_builddir)/src/python/libadonthell_python.la \
    $(top_builddir)/src/world/libadonthell_world.la \
    $(glog_LIBS) 
 



###### Following definitions are for the backends
pkglibmaindir = $(pkglibdir)/main
pkglibmain_LTLIBRARIES = _sdl.la


### SDL backend

## MacOS X specific files
if OSX_COMPILE
_sdl_hdrs = sdl/osx.h
_sdl_la_SOURCES = sdl/osx.m $(_sdl_hdrs)
_sdl_la_OBJCFLAGS = $(AM_CXXFLAGS) $(SDL_CFLAGS) -ObjC++
_sdl_la_LDFLAGS = -module -avoid-version -Wl,-framework,Cocoa -lobjc
## Files specific to Linux (and others)
else
_sdl_la_SOURCES = sdl/linux.cc
_sdl_la_LDFLAGS = -module -avoid-version
endif

_sdl_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" $(SDL_CFLAGS) $(AM_CXXFLAGS)
_sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_main -lstdc++

## define dependencies in case of parallel build
_sdl_la_DEPENDENCIES = libadonthell_main.la
