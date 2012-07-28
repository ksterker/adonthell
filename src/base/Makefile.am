backenddir = @backenddir@
AM_CXXFLAGS = -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

adonthellincludedir = $(pkgincludedir)-@VERSION@/adonthell

## Our header files
pkgincludebasedir = $(adonthellincludedir)/base
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
    logging.h \
    nls.h \
	paths.h \
    savegame.h \
    serializer.h \
	timer.h \
	types.h \
    utf8.h

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
    logging.cc \
    nls.cc \
	paths.cc \
    savegame.cc \
	timer.cc \
    utf8.cc

libadonthell_base_la_CXXFLAGS = $(XML_CPPFLAGS) $(XML_CFLAGS) $(libgmock_CFLAGS) \
        -DPKGLIBDIR=\"$(backenddir)\" $(AM_CXXFLAGS)
libadonthell_base_la_LIBADD = $(XML_LIBS) $(libglog_LIBS) $(LIBS)

## Unit tests
test_CXXFLAGS = $(libgmock_CFLAGS) $(libgtest_CFLAGS)
test_LDADD    = $(libgmock_LIBS)   $(libgtest_LIBS)

test_logging_SOURCES  = $(libadonthell_base_la_SOURCES)  test_logging.cc
test_logging_CXXFLAGS = $(libadonthell_base_la_CXXFLAGS) $(test_CXXFLAGS)
test_logging_LDADD    = $(libadonthell_base_la_LIBADD)   $(test_LDADD)

TESTS          = test_logging
check_PROGRAMS = $(TESTS)
