CXXFLAGS += -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

## Our header files
pkgincludegfxdir = $(pkgincludedir)/gfx
pkgincludegfx_HEADERS = \
	animation.h \
	drawable.h \
	drawing_area.h \
	gfx.h \
    image.h \
	png_wrapper.h \
	screen.h \
	surface.h

## Main library
lib_LTLIBRARIES = libadonthell_gfx.la

## Rules to build libgfx
libadonthell_gfx_la_SOURCES = \
	animation.cc \
	drawable.cc \
	drawing_area.cc \
	gfx.cc \
    image.cc \
	png_wrapper.cc \
	screen.cc \
	surface.cc

libadonthell_gfx_la_CXXFLAGS = $(PY_CFLAGS)
libadonthell_gfx_la_LIBADD = $(PY_LIBS) -L$(top_builddir)/src/base/ -ladonthell_base -lstdc++ -lpng



###### Following definitions are for the backends
pkglibgfxdir = $(pkglibdir)/gfx
pkglibgfx_LTLIBRARIES = _sdl.la 
#sdlgl.la


### SDL backend

## Our header files
pkgincludegfxsdldir = $(pkgincludedir)/gfx/sdl
pkgincludegfxsdl_HEADERS = \
	sdl/screen_sdl.h \
	sdl/surface_sdl.h

## Rules to build libgfx_sdl
_sdl_la_SOURCES = \
	sdl/gfx_sdl.cc \
	sdl/screen_sdl.cc \
	sdl/surface_sdl.cc

_sdl_la_CXXFLAGS = $(SDL_CFLAGS)
_sdl_la_LDFLAGS = -module -avoid-version
_sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_gfx

### SDLGL backend

## Our header files
#pkgincludegfxsdlgldir = $(pkgincludedir)/gfx/sdlgl
#pkgincludegfxsdlgl_HEADERS = \
#	sdlgl/screen_sdlgl.h \
#	sdlgl/surface_sdlgl.h

## Rules to build libgfx_sdl
#sdlgl_la_SOURCES = \
#	sdlgl/gfx_sdlgl.cc \
#	sdlgl/screen_sdlgl.cc \
#	sdlgl/surface_sdlgl.cc

#sdlgl_la_CXXFLAGS = $(SDL_CFLAGS)
#sdlgl_la_LDFLAGS = -module -avoid-version
#sdlgl_la_LIBADD = $(SDL_LIBS) -ladonthell_gfx -lGLU -lGL
