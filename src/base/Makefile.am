CXXFLAGS += -I$(top_srcdir)/src/

## Our header files
pkgincludebasedir = $(pkgincludedir)/base
pkgincludebase_HEADERS = \
	base.h \
	callback.h \
    configuration.h \
    configio.h \
    diskio.h \
	endians.h \
	file.h \
	flat.h \
    hash_map.h \
	paths.h \
	timer.h \
	types.h

## Main library
lib_LTLIBRARIES = libadonthell_base.la

## Rules to build libbase
libadonthell_base_la_SOURCES = \
    base.cc \
	callback.cc \
    configuration.cc \
    configio.cc \
    diskio.cc \
	file.cc \
	flat.cc \
	paths.cc \
	timer.cc

libadonthell_base_la_CXXFLAGS = $(XML_CFLAGS) -DPKGLIBDIR=\"$(pkglibdir)\"
libadonthell_base_la_LIBADD = $(XML_LIBS) $(LIBLTDL) -lz
