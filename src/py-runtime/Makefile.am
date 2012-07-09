AM_CXXFLAGS = -I$(top_srcdir)/src
EXTRA_DIST = py_runtime.i CMakeLists.txt
CLEANFILES = $(top_srcdir)/src/py-runtime/py_runtime.cc

## SWIG runtime support
lib_LTLIBRARIES = libadonthell_py_runtime.la

## the swig runtime code
py_runtime.cc: py_runtime.i Makefile.am
	$(SWIG) -c++ -noexcept -nocontract -python -modern -module py_runtime -o $(srcdir)/$@ $<

## Rules to build libadonthell_py_runtime
libadonthell_py_runtime_la_SOURCES = py_runtime.cc
libadonthell_py_runtime_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS) -DSWIGRUNTIME= -DSWIGINLINE=
libadonthell_py_runtime_la_LIBADD = $(PY_LIBS)
