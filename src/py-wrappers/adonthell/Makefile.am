CXXFLAGS += -I$(top_srcdir)/src/
EXTRA_DIST = py_base.i py_event.i py_gfx.i py_input.i py_gui.i py_main.i

## Python wrapper files.
pkgpyexec_PYTHON = __init__.py base.py event.py gfx.py input.py gui.py main.py

## Python helper libraries
pkgpyexec_LTLIBRARIES = libswigruntime.la _base.la _event.la _gfx.la _input.la _gui.la _main.la

## Rules to build SWIG's runtime library
libswigruntime_la_SOURCES = swigruntime_wrap.cc
libswigruntime_la_CXXFLAGS = $(PY_CFLAGS)
#-lswigruntime_LDFLAGS = -avoid-version -module
libswigruntime_la_LIBADD = $(PY_LIBS)

## Rules to build libpybase (libbase's Python wrapper)
_base_la_SOURCES = py_base_wrap.cc
_base_la_CXXFLAGS = $(PY_CFLAGS)
_base_la_LDFLAGS = -avoid-version -module
_base_la_LIBADD = -L$(top_builddir)/src/base/ -ladonthell_base -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) -lswigruntime

## Rules to build libpyevent (libevent's Python wrapper)
_event_la_SOURCES = py_event_wrap.cc
_event_la_CXXFLAGS = $(PY_CFLAGS)
_event_la_LDFLAGS = -avoid-version -module
_event_la_LIBADD = -L$(top_builddir)/src/event/ -ladonthell_event -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) -lswigruntime

## Rules to build libpygfx (libgfx's Python wrapper)
_gfx_la_SOURCES = py_gfx_wrap.cc
_gfx_la_CXXFLAGS = $(PY_CFLAGS)
_gfx_la_LDFLAGS = -avoid-version -module
_gfx_la_LIBADD = -L$(top_builddir)/src/gfx/ -ladonthell_gfx -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) -lswigruntime

## Rules to build libpyinput (libinput's Python wrapper)
_input_la_SOURCES = py_input_wrap.cc
_input_la_CXXFLAGS = $(PY_CFLAGS)
_input_la_LDFLAGS = -avoid-version -module
_input_la_LIBADD = -L$(top_builddir)/src/input/ -ladonthell_input -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) -lswigruntime

## Rules to build libpygui (libgui's Python wrapper)
_gui_la_SOURCES = py_gui_wrap.cc
_gui_la_CXXFLAGS = $(PY_CFLAGS)
_gui_la_LDFLAGS = -avoid-version -module
_gui_la_LIBADD = -L$(top_builddir)/src/gui/ -ladonthell_gui -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) -lswigruntime

## Rules to build libpymain (libmain's Python wrapper)
_main_la_SOURCES = py_main_wrap.cc py_main.h $(top_srcdir)/src/main/adonthell.cc
_main_la_CXXFLAGS = $(PY_CFLAGS)
_main_la_LDFLAGS = -avoid-version -module
_main_la_LIBADD = -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS) -lswigruntime


## Rules for SWIG runtime library generation
swigruntime_wrap.cc:
	swig -runtime -python -noproxy -o $(srcdir)/$@ swigrun.i

## Rules for Python wrappers generation
base.py: py_base_wrap.cc
py_base_wrap.cc: py_base.i $(top_srcdir)/src/base/*.h
	$(SWIG) -noruntime -python -I$(top_srcdir)/src -o $(srcdir)/$@ $<

event.py: py_event_wrap.cc
py_event_wrap.cc: py_event.i $(top_srcdir)/src/event/*.h
	$(SWIG) -noruntime -python -I$(top_srcdir)/src -o $(srcdir)/$@ $<

gfx.py: py_gfx_wrap.cc
py_gfx_wrap.cc: py_gfx.i $(top_srcdir)/src/gfx/*.h
	$(SWIG) -noruntime -python -I$(top_srcdir)/src -o $(srcdir)/$@ $<

input.py: py_input_wrap.cc
py_input_wrap.cc: py_input.i $(top_srcdir)/src/input/*.h
	$(SWIG) -noruntime -python -I$(top_srcdir)/src -o $(srcdir)/$@ $<

gui.py: py_gui_wrap.cc
py_gui_wrap.cc: py_gui.i $(top_srcdir)/src/gui/*.h
	$(SWIG) -noruntime -python -I$(top_srcdir)/src -o $(srcdir)/$@ $<

main.py: py_main_wrap.cc
py_main_wrap.cc: py_main.i $(top_srcdir)/src/main/*.h
	$(SWIG) -noruntime -python -I$(top_srcdir)/src -I$(top_srcdir)/src/py-wrappers/adonthell -o $(srcdir)/$@ $<

__init__.py:
	touch $(srcdir)/$@
