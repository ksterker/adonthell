CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

## Our header files
pkgincludeeventdir = $(pkgincludedir)/event
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

libadonthell_event_la_CXXFLAGS = $(PY_CFLAGS)
libadonthell_event_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base $(SWIGRUNTIME_LIBS) -lstdc++
