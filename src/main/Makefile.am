AM_CXXFLAGS = -I$(top_srcdir)/src
EXTRA_DIST = CMakeLists.txt sdl/linux.cc sdl/winnt.cc sdl/osx.h sdl/osx.m

## Our header files
pkgincludemaindir = $(pkgincludedir)/main/
pkgincludemain_HEADERS = \
	adonthell.h
    
## Main library
lib_LTLIBRARIES = libadonthell_main.la

## Rules to build libmain
libadonthell_main_la_SOURCES = \
	adonthell.cc \
	main.cc

libadonthell_main_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_main_la_LIBADD = $(PY_LIBS)


###### Following definitions are for the backends
pkglibmaindir = $(pkglibdir)/main
pkglibmain_LTLIBRARIES = _sdl.la


### SDL backend

## MacOS X specific files
if OSX_COMPILE
_sdl_hdrs = sdl/osx.h
_sdl_la_SOURCES = sdl/osx.m $(_sdl_hdrs)
_sdl_la_OBJCFLAGS = -ObjC++ $(AM_CXXFLAGS) $(SDL_CFLAGS) 

## Files specific to Linux (and others)
else
_sdl_la_SOURCES = sdl/linux.cc
endif

_sdl_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" $(SDL_CFLAGS) $(AM_CXXFLAGS)
_sdl_la_LDFLAGS = -module -avoid-version
_sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_main -lstdc++
