CXXFLAGS += -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

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

libadonthell_input_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\"
libadonthell_input_la_LIBADD = -L$(top_builddir)/src/base/ -ladonthell_base -lstdc++



###### Following definitions are for the backends
pkglibinputdir = $(pkglibdir)/input
pkglibinput_LTLIBRARIES = _sdl.la

### SDL backend
## Our header files
pkgincludeinputsdldir = $(pkgincludedir)/input/sdl
pkgincludeinputsdl_HEADERS = \
	sdl/manager_sdl.h

## Rules to build libinput_sdl
_sdl_la_SOURCES = \
	sdl/input_sdl.cc \
	sdl/manager_sdl.cc

_sdl_la_CXXFLAGS = $(SDL_CFLAGS)
_sdl_la_LDFLAGS = -avoid-version -module
_sdl_la_LIBADD = $(SDL_LIBS) -ladonthell_input -lstdc++
