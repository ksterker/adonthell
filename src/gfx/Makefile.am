CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

## Our header files
pkgincludegfxdir = $(pkgincludedir)/gfx
pkgincludegfx_HEADERS = \
	gfx.h \
	drawing_area.h \
	drawable.h \
	pnm.h \
	surface.h \
	screen.h

## Main library
lib_LTLIBRARIES = libadonthell_gfx.la

## Rules to build libgfx
libadonthell_gfx_la_SOURCES = \
	gfx.cc \
	drawing_area.cc \
	drawable.cc \
	pnm.cc \
	surface.cc \
	screen.cc
libadonthell_gfx_la_LIBADD = -L$(top_builddir)/src/base/ -ladonthell_base -lstdc++



###### Following definitions are for the backends
pkglibgfxdir = $(pkglibdir)/gfx
pkglibgfx_LTLIBRARIES = sdl.la

### SDL backend
## Our header files
pkgincludegfxsdldir = $(pkgincludedir)/gfx/sdl
pkgincludegfxsdl_HEADERS = \
	sdl/screen_sdl.h \
	sdl/surface_sdl.h 

## Rules to build libgfx_sdl
sdl_la_SOURCES = \
	sdl/gfx_sdl.cc \
	sdl/screen_sdl.cc \
	sdl/surface_sdl.cc 

sdl_la_CXXFLAGS = $(SDL_CFLAGS)
sdl_la_LDFLAGS = -module -avoid-version
sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_gfx
