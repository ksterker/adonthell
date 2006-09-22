CXXFLAGS += -I$(top_srcdir)/src

## Our header files
pkgincludebasedir = $(pkgincludedir)/base
pkgincludebase_HEADERS = \
	base.h \
	callback.h \
    configuration.h \
    configio.h \
    diskio.h \
    diskwriter_base.h \
    diskwriter_gz.h \
    diskwriter_xml.h \
	endians.h \
	file.h \
	flat.h \
	gettext.h \
    hash_map.h \
    nls.h \
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
    diskwriter_gz.cc \
    diskwriter_xml.cc \
	file.cc \
	flat.cc \
    nls.cc \
	paths.cc \
	timer.cc

libadonthell_base_la_CXXFLAGS = $(XML_CPPFLAGS) $(XML_CFLAGS) -DPKGLIBDIR=\"$(pkglibdir)\"
libadonthell_base_la_LIBADD = $(XML_LIBS) $(LIBLTDL) -lz
