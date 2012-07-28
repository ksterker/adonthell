AM_CXXFLAGS = -I$(top_builddir)
DEFAULT_INCLUDES = -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

adonthellincludedir = $(pkgincludedir)-@VERSION@/adonthell

## Our header files
pkgincludeaudiodir = $(adonthellincludedir)/audio
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

libadonthell_audio_la_CXXFLAGS = $(PY_CFLAGS) $(libglog_CFLAGS) \
        -DPKGLIBDIR=\"$(pkglibdir)\" $(AM_CXXFLAGS)
libadonthell_audio_la_LIBADD   = $(PY_LIBS)   $(libglog_LIBS)   \
        -lltdl -lstdc++ \
		$(top_builddir)/src/base/libadonthell_base.la \
		$(top_builddir)/src/event/libadonthell_event.la



###### Following definitions are for the backends
pkglibaudiodir = $(pkglibdir)/audio
pkglibaudio_LTLIBRARIES = _sdl.la

### SDL backend
## Our header files
pkgincludeaudiosdldir = $(adonthellincludedir)/audio/sdl

## Rules to build libaudio_sdl
_sdl_la_SOURCES = \
	sdl/audio_sdl.cc

_sdl_la_CXXFLAGS = $(SDL_CFLAGS) $(AM_CXXFLAGS)
_sdl_la_LDFLAGS = -avoid-version -module
_sdl_la_LIBADD = $(SDL_LIBS) $(SDL_MIXER_LIBS) -ladonthell_audio -lstdc++

## define dependencies in case of parallel build
_sdl_la_DEPENDENCIES = libadonthell_audio.la

## Unit tests
test_CXXFLAGS = $(libgmock_CFLAGS) $(libgtest_CFLAGS)
test_LDADD    = $(libgmock_LIBS)   $(libgtest_LIBS) $(top_builddir)/src/audio/libadonthell_audio.la

test_audio_SOURCES  = test_audio.cc
test_audio_CXXFLAGS = $(libadonthell_audio_la_CXXFLAGS) $(test_CXXFLAGS)
test_audio_LDADD    = $(libadonthell_audio_la_LIBADD)   $(test_LDADD)

test_audio_event_SOURCES  = test_audio_event.cc
test_audio_event_CXXFLAGS = $(libadonthell_audio_la_CXXFLAGS) $(test_CXXFLAGS)
test_audio_event_LDADD    = $(libadonthell_audio_la_LIBADD)   $(test_LDADD)

test_audio_manager_SOURCES  = test_audio_manager.cc
test_audio_manager_CXXFLAGS = $(libadonthell_audio_la_CXXFLAGS) $(test_CXXFLAGS)
test_audio_manager_LDADD    = $(libadonthell_audio_la_LIBADD)   $(test_LDADD)

test_sound_SOURCES  = test_sound.cc
test_sound_CXXFLAGS = $(libadonthell_audio_la_CXXFLAGS) $(test_CXXFLAGS)
test_sound_LDADD    = $(libadonthell_audio_la_LIBADD)   $(test_LDADD)

TESTS = \
	test_audio         \
	test_audio_event   \
	test_audio_manager \
	test_sound

check_PROGRAMS = $(TESTS)
