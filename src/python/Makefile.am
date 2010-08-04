AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

## Our header files
pkgincludepythondir = $(pkgincludedir)/python
pkgincludepython_HEADERS = \
	callback_support.h \
	callback.h \
	method.h \
	pool.h \
	python.h \
	script.h

## Main library
lib_LTLIBRARIES = libadonthell_python.la

## Rules to build libpython
libadonthell_python_la_SOURCES = \
	callback.cc \
	method.cc \
	pool.cc \
	python.cc \
	script.cc

libadonthell_python_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_python_la_LIBADD = $(PY_LIBS) \
    $(top_builddir)/src/base/libadonthell_base.la \
    $(SWIGRUNTIME_LIBS) -lstdc++


## Unit tests
test_CXXFLAGS = $(libgmock_CFLAGS) $(libgtest_CFLAGS)
test_LDADD    = $(libgmock_LIBS)   $(libgtest_LIBS) $(top_builddir)/src/python/libadonthell_python.la

test_python_SOURCES  = test_python.cc
test_python_CXXFLAGS = $(libadonthell_python_la_CXXFLAGS) $(test_CXXFLAGS)
test_python_LDADD    = $(libadonthell_python_la_LIBADD)   $(test_LDADD)

TESTS = \
	test_python

check_PROGRAMS = $(TESTS)
