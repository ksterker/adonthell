CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

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


###### Following definitions are for the backends
pkglibmaindir = $(pkglibdir)/main
pkglibmain_LTLIBRARIES = sdl.la


### SDL backend

## MacOS X specific files
if OSX_COMPILE
sdl_hdrs = sdl/osx.h
sdl_la_SOURCES = sdl/osx.m $(sdl_hdrs) 

## Files specific to Linux (and others)
else
sdl_la_SOURCES = sdl/linux.cc
endif

sdl_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" $(SDL_CFLAGS)
sdl_la_OBJCFLAGS = -g -ObjC++ $(SDL_CFLAGS)
sdl_la_LDFLAGS = -module -avoid-version
sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_main -lstdc++
