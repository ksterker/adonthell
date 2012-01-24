AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt sdl/manager_sdl.h sdl/input_sdl.cc \
	sdl/manager_sdl.cc sdl2/manager_sdl.h sdl2/input_sdl.cc \
	sdl2/manager_sdl.cc


## Our header files
pkgincludeinputdir = $(pkgincludedir)/input
pkgincludeinput_HEADERS = \
	input.h \
	control_event.h \
	keyboard_event.h \
	mouse_event.h \
	joystick_event.h \
	event.h \
	listener.h \
	manager.h

## Main library
lib_LTLIBRARIES = libadonthell_input.la

libadonthell_input_la_SOURCES = \
	input.cc \
	event.cc \
	joystick_event.cc \
	mouse_event.cc \
	keyboard_event.cc \
	control_event.cc \
	listener.cc \
	manager.cc

libadonthell_input_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" $(AM_CXXFLAGS)
libadonthell_input_la_LIBADD = $(top_builddir)/src/base/libadonthell_base.la -lltdl -lstdc++



###### Following definitions are for the backends
pkglibinputdir = $(pkglibdir)/input
pkglibinput_LTLIBRARIES = _sdl.la

### SDL backend
## Our header files
pkgincludeinputsdldir = $(pkgincludedir)/input/sdl
pkgincludeinputsdl_HEADERS = \
    $(sdlbackenddir)/manager_sdl.h

## Rules to build libinput_sdl
_sdl_la_SOURCES = \
	$(sdlbackenddir)/input_sdl.cc \
	$(sdlbackenddir)/manager_sdl.cc

_sdl_la_CXXFLAGS = $(SDL_CFLAGS) $(AM_CXXFLAGS)
_sdl_la_LDFLAGS = -avoid-version -module
_sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_input -lstdc++

## define dependencies in case of parallel build
_sdl_la_DEPENDENCIES = libadonthell_input.la
