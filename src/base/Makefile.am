CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

## Our header files
pkgincludebasedir = $(pkgincludedir)/base
pkgincludebase_HEADERS = \
	types.h \
	base.h \
	paths.h \
	callback.h \
	timer.h

## Main library
lib_LTLIBRARIES = libadonthell_base.la

## Rules to build libbase
libadonthell_base_la_SOURCES = \
	paths.cc \
	callback.cc \
	timer.cc

libadonthell_base_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\"
libadonthell_base_la_LIBADD = $(LIBLTDL) -lz 
