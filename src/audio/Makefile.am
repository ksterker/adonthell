AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

## Our header files
pkgincludeaudiodir = $(pkgincludedir)/audio
pkgincludeaudio_HEADERS = \
	audio.h \
	audio_event.h \
	audio_event_manager.h \
	sound.h

## Main library
lib_LTLIBRARIES = libadonthell_audio.la

libadonthell_audio_la_SOURCES = \
	audio.cc \
	sound.cc \
	audio_event.cc \
	audio_event_manager.cc

libadonthell_audio_la_CXXFLAGS = $(PY_CFLAGS) -DPKGLIBDIR=\"$(pkglibdir)\" $(AM_CXXFLAGS)
libadonthell_audio_la_LIBADD = $(PY_LIBS) -lstdc++ \
		-L$(top_builddir)/src/base/ -ladonthell_base \
		-L$(top_builddir)/src/event/ -ladonthell_event



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
_sdl_la: libadonthell_audio_la