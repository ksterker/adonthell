CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

## Our header files
pkgincludeeventdir = $(pkgincludedir)/event
pkgincludeevent_HEADERS = \
	date.h 
#    event.h \
#    handler.h \
#    handler_base.h \
#    list.h \
#    time_event.h \
#    time_event_handler.h
    
## Main library
lib_LTLIBRARIES = libadonthell_event.la

## Rules to build libpython
libadonthell_event_la_SOURCES = \
	date.cc 
#    event.cc \
#    handler.cc \
#    list.cc \
#    time_event.cc \
#    time_event_handler.cc

    
# libadonthell_event_la_CXXFLAGS =
libadonthell_event_la_LIBADD = -L../base -ladonthell_base $(SWIGRUNTIME_LIBS) -lstdc++
