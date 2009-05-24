AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt sdlgl/screen_sdlgl.h sdlgl/surface_sdlgl.h sdlgl/gfx_sdlgl.cc \
	sdlgl/screen_sdlgl.cc sdlgl/surface_sdlgl.cc sdl/screen_sdl.h sdl/surface_sdl.h \
	sdl/gfx_sdl.cc sdl/screen_sdl.cc sdl/surface_sdl.cc sdl13/screen_sdl.h sdl13/surface_sdl.h \
	sdl13/gfx_sdl.cc sdl13/screen_sdl.cc sdl13/surface_sdl.cc


## Our header files
pkgincludegfxdir = $(pkgincludedir)/gfx
pkgincludegfx_HEADERS = \
	drawable.h \
	drawing_area.h \
	gfx.h \
    image.h \
	png_wrapper.h \
	screen.h \
    sprite.h \
	surface.h \
    surface_ext.h \
    surface_cacher.h

## Main library
lib_LTLIBRARIES = libadonthell_gfx.la

## Rules to build libgfx
libadonthell_gfx_la_SOURCES = \
	drawable.cc \
	drawing_area.cc \
	gfx.cc \
    image.cc \
	png_wrapper.cc \
	screen.cc \
    sprite.cc \
	surface.cc \
    surface_ext.cc \
    surface_cacher.cc


libadonthell_gfx_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_gfx_la_LIBADD = $(PY_LIBS) -lltdl \
    -L$(top_builddir)/src/base/ -ladonthell_base \
    -L$(top_builddir)/src/event/ -ladonthell_event \
    -lstdc++ -lpng



###### Following definitions are for the backends
pkglibgfxdir = $(pkglibdir)/gfx
pkglibgfx_LTLIBRARIES = _sdl.la
#sdlgl.la


### SDL backend

## Our header files
noinst_HEADERS = \
	$(sdlbackenddir)/screen_sdl.h \
	$(sdlbackenddir)/surface_sdl.h

## Rules to build libgfx_sdl
_sdl_la_SOURCES = \
	$(sdlbackenddir)/gfx_sdl.cc \
	$(sdlbackenddir)/screen_sdl.cc \
	$(sdlbackenddir)/surface_sdl.cc

_sdl_la_CXXFLAGS = $(SDL_CFLAGS) $(AM_CXXFLAGS)
_sdl_la_LDFLAGS = -module -avoid-version
_sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_gfx

## define dependencies in case of parallel build
_sdl_la: libadonthell_gfx_la

### SDLGL backend

## Our header files
#noinst_HEADERS = \
#	sdlgl/screen_sdlgl.h \
#	sdlgl/surface_sdlgl.h

## Rules to build libgfx_sdl
#sdlgl_la_SOURCES = \
#	sdlgl/gfx_sdlgl.cc \
#	sdlgl/screen_sdlgl.cc \
#	sdlgl/surface_sdlgl.cc

#sdlgl_la_CXXFLAGS = $(SDL_CFLAGS) $(AM_CXXFLAGS)
#sdlgl_la_LDFLAGS = -module -avoid-version
#sdlgl_la_LIBADD = $(SDL_LIBS) -ladonthell_gfx -lGLU -lGL
