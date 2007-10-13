AM_CXXFLAGS = -I$(top_srcdir)/src
EXTRA_DIST = py_runtime.i py_debug.i CMakeLists.txt
CLEANFILES = $(top_srcdir)/src/py-wrappers/runtime/py_*_wrap.cc

## Python wrapper files.
pkgpyexec_PYTHON = __init__.py debug.py
noinst_SCRIPTS = __init__.pyc debug.pyc

## Python helper libraries
pkgpyexec_LTLIBRARIES = _debug.la

## SWIG runtime support
lib_LTLIBRARIES = libadonthell_py_runtime.la

## the swig runtime code
py_runtime.cc: py_runtime.i Makefile.am
	$(SWIG) -c++ -noexcept -nocontract -python -modern -module py_runtime -o $(srcdir)/$@ $<

## Rules to build libadonthell_py_runtime
libadonthell_py_runtime_la_SOURCES = py_runtime.cc
libadonthell_py_runtime_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS) -DSWIGRUNTIME= -DSWIGINLINE=
libadonthell_py_runtime_la_LIBADD = $(PY_LIBS)

PY_LIBADD = -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS)

## Rules to build libpydebug (helper for debugging the engine)
_debug_la_SOURCES = py_debug_wrap.cc
_debug_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_debug_la_LDFLAGS = -avoid-version -module
_debug_la_LIBADD = $(PY_LIBADD) -L$(top_builddir)/src/python -ladonthell_python -ladonthell_py_runtime


## Swig arguments
SWIG_ARGS = -python -modern -noexcept -I$(top_srcdir)/src

## Rules for Python wrappers generation

debug.py: py_debug_wrap.cc
py_debug_wrap.cc: py_debug.i py_runtime.i
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

__init__.py:
	touch ./$@

## The .pyc are generated so the Python modules are accessible even if
## we are using a separate builddir.
%.pyc: %.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"

