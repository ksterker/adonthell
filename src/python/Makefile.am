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
libadonthell_python_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base $(SWIGRUNTIME_LIBS) -lstdc++
