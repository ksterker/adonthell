AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

## Our header files
pkgincludeaudiodir = $(pkgincludedir)/audio
pkgincludeaudio_HEADERS = \
	audio.h \
	audio_event.h \
	audio_event_manager.h \
	audio_manager.h \
	sound.h

## Main library
lib_LTLIBRARIES = libadonthell_audio.la

libadonthell_audio_la_SOURCES = \
	audio.cc \
	sound.cc \
	audio_event.cc \
	audio_event_manager.cc \
	audio_manager.cc

libadonthell_audio_la_CXXFLAGS = $(PY_CFLAGS) -DPKGLIBDIR=\"$(pkglibdir)\" $(AM_CXXFLAGS)
libadonthell_audio_la_LIBADD = $(PY_LIBS) -lltdl -lstdc++ \
		$(top_builddir)/src/base/libadonthell_base.la \
		$(top_builddir)/src/event/libadonthell_event.la



###### Following definitions are for the backends
pkglibaudiodir = $(pkglibdir)/audio
pkglibaudio_LTLIBRARIES = _sdl.la

### SDL backend
## Our header files
pkgincludeaudiosdldir = $(pkgincludedir)/audio/sdl

## Rules to build libaudio_sdl
_sdl_la_SOURCES = \
	sdl/audio_sdl.cc

_sdl_la_CXXFLAGS = $(SDL_CFLAGS) $(AM_CXXFLAGS)
_sdl_la_LDFLAGS = -avoid-version -module
_sdl_la_LIBADD = $(SDL_LIBS) -lSDL_mixer -ladonthell_audio -lstdc++

## define dependencies in case of parallel build
_sdl_la_DEPENDENCIES = libadonthell_audio.la

## Unit tests
check_PROGRAMS = test_audio_manager

test_audio_manager_SOURCES  = $(libadonthell_audio_la_SOURCES) test_audio_manager.cc
test_audio_manager_CXXFLAGS = $(libadonthell_audio_la_CXXFLAGS)
test_audio_manager_LDADD    = $(libadonthell_audio_la_LIBADD) -lgtest

TESTS = test_audio_manager
