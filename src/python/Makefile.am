CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

## Our header files
pkgincludepythondir = $(pkgincludedir)/python
pkgincludepython_HEADERS = \
	python.h \
	callback_support.h \
	callback.h

## Main library
lib_LTLIBRARIES = libadonthell_python.la

## Rules to build libpython
libadonthell_python_la_SOURCES = \
	python.cc \
	callback.cc

libadonthell_python_la_CXXFLAGS = $(PY_CFLAGS)
libadonthell_python_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base $(SWIGRUNTIME_LIBS) -lstdc++
