CXXFLAGS += -I$(top_srcdir)/src/
EXTRA_DIST = py_base.i py_gfx.i py_input.i py_gui.i

## Python wrapper files.
pkgpyexec_PYTHON = __init__.py base.py gfx.py input.py gui.py

## Python helper libraries
pkgpyexec_LTLIBRARIES = _base.la _gfx.la _input.la _gui.la

## Rules to build libpybase (libbase's Python wrapper)
_base_la_SOURCES = py_base_wrap.cc
_base_la_CXXFLAGS = $(PY_CFLAGS)
_base_la_LDFLAGS = -avoid-version -module
_base_la_LIBADD = -L$(top_builddir)/src/base/ -ladonthell_base -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) $(SWIGRUNTIME_LIBS)

## Rules to build libpygfx (libgfx's Python wrapper)
_gfx_la_SOURCES = py_gfx_wrap.cc
_gfx_la_CXXFLAGS = $(PY_CFLAGS)
_gfx_la_LDFLAGS = -avoid-version -module
_gfx_la_LIBADD = -L$(top_builddir)/src/gfx/ -ladonthell_gfx -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) $(SWIGRUNTIME_LIBS)

## Rules to build libpyinput (libinput's Python wrapper)
_input_la_SOURCES = py_input_wrap.cc
_input_la_CXXFLAGS = $(PY_CFLAGS)
_input_la_LDFLAGS = -avoid-version -module
_input_la_LIBADD = -L$(top_builddir)/src/input/ -ladonthell_input -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) $(SWIGRUNTIME_LIBS)


## Rules to build libpygui (libgui's Python wrapper)
_gui_la_SOURCES = py_gui_wrap.cc
_gui_la_CXXFLAGS = $(PY_CFLAGS)
_gui_la_LDFLAGS = -avoid-version -module
_gui_la_LIBADD = -L$(top_builddir)/src/gui/ -ladonthell_gui -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) $(SWIGRUNTIME_LIBS)


## Rules for Python wrappers generation
base.py: py_base_wrap.cc
py_base_wrap.cc: py_base.i $(top_srcdir)/src/base/*.h
	$(SWIG) -c -python -shadow -I$(top_srcdir)/src -o $(srcdir)/$@ $<

gfx.py: py_gfx_wrap.cc
py_gfx_wrap.cc: py_gfx.i $(top_srcdir)/src/gfx/*.h
	$(SWIG) -c -python -shadow -I$(top_srcdir)/src -o $(srcdir)/$@ $<

input.py: py_input_wrap.cc
py_input_wrap.cc: py_input.i $(top_srcdir)/src/input/*.h
	$(SWIG) -c -python -shadow -I$(top_srcdir)/src -o $(srcdir)/$@ $<

gui.py: py_gui_wrap.cc
py_gui_wrap.cc: py_gui.i $(top_srcdir)/src/gui/*.h
	$(SWIG) -c -python -shadow -I$(top_srcdir)/src -o $(srcdir)/$@ $<

__init__.py:
	touch $(srcdir)/$@
