AM_CXXFLAGS = -I$(top_builddir)
DEFAULT_INCLUDES = -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

adonthellincludedir = $(pkgincludedir)-@VERSION@/adonthell

## Our header files
pkgincludeeventdir = $(adonthellincludedir)/event
pkgincludeevent_HEADERS = \
	date.h \
	event.h \
	factory.h \
	listener.h \
	listener_cxx.h \
	listener_python.h \
	manager.h \
	manager_base.h \
	time_event.h \
	time_event_manager.h \
    types.h
    
## Main library
lib_LTLIBRARIES = libadonthell_event.la

## Rules to build libevent
libadonthell_event_la_SOURCES = \
	date.cc \
	event.cc \
	factory.cc \
	listener.cc \
	listener_cxx.cc \
	listener_python.cc \
    types.cc \
	time_event.cc \
	time_event_manager.cc

libadonthell_event_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_event_la_LIBADD = $(PY_LIBS) \
    $(top_builddir)/src/base/libadonthell_base.la \
    $(top_builddir)/src/python/libadonthell_python.la \
    $(top_builddir)/src/py-runtime/libadonthell_py_runtime.la -lstdc++
