CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

## Our header files
pkgincludebasedir = $(pkgincludedir)/base
pkgincludebase_HEADERS = \
	base.h \
	callback.h \
    diskio.h \
	endian.h \
	file.h \
	flat.h \
	paths.h \
	timer.h \
	types.h

## Main library
lib_LTLIBRARIES = libadonthell_base.la

## Rules to build libbase
libadonthell_base_la_SOURCES = \
	callback.cc \
    diskio.cc \
	file.cc \
	flat.cc \
	paths.cc \
	timer.cc

libadonthell_base_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\"
libadonthell_base_la_LIBADD = $(LIBLTDL) -lz 
