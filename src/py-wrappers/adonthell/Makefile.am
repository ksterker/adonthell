AM_CXXFLAGS = -I$(top_srcdir)/src
EXTRA_DIST = py_debug.i py_base.i py_event.i py_gfx.i py_gui.i py_input.i py_audio.i py_main.i py_rpg.i py_world.i CMakeLists.txt
CLEANFILES = $(top_srcdir)/src/py-wrappers/adonthell/py_debug_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_base_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_event_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_gfx_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_gui_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_input_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_audio_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_main_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_rpg_wrap.cc \
    $(top_srcdir)/src/py-wrappers/adonthell/py_world_wrap.cc

## Python wrapper files.
pkgpyexec_PYTHON = __init__.py debug.py base.py event.py gfx.py gui.py input.py audio.py main.py rpg.py world.py
noinst_SCRIPTS = __init__.pyc debug.pyc base.pyc event.pyc gfx.pyc gui.pyc input.pyc audio.pyc main.pyc rpg.pyc world.pyc

## Python helper libraries
pkgpyexec_LTLIBRARIES = _debug.la _base.la _event.la _gfx.la _gui.la _input.la _audio.la  _main.la _rpg.la _world.la

## SWIG runtime support
lib_LTLIBRARIES = libadonthell_py_main.la

PY_LIBADD = $(top_builddir)/src/python/libadonthell_python.la $(PY_LIBS)
PY_RUNTIME = $(top_builddir)/src/py-runtime/libadonthell_py_runtime.la

## Rules to build libadonthell_py_main
libadonthell_py_main_la_SOURCES = py_main.cc py_main.h
libadonthell_py_main_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_py_main_la_LIBADD = $(PY_LIBADD) \
	$(top_builddir)/src/main/libadonthell_main.la \
	$(top_builddir)/src/base/libadonthell_base.la
	
## Rules to build libpybase (libbase's Python wrapper)
_base_la_SOURCES = py_base_wrap.cc
_base_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_base_la_LDFLAGS = -avoid-version -module
_base_la_LIBADD = $(PY_LIBADD) $(top_builddir)/src/base/libadonthell_base.la  $(PY_RUNTIME)

## Rules to build libpyevent (libevent's Python wrapper)
_event_la_SOURCES = py_event_wrap.cc
_event_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_event_la_LDFLAGS = -avoid-version -module
_event_la_LIBADD = $(PY_LIBADD) $(top_builddir)/src/event/libadonthell_event.la $(PY_RUNTIME)

## Rules to build libpygfx (libgfx's Python wrapper)
_gfx_la_SOURCES = py_gfx_wrap.cc
_gfx_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_gfx_la_LDFLAGS = -avoid-version -module
_gfx_la_LIBADD = $(PY_LIBADD) \
	$(top_builddir)/src/gfx/libadonthell_gfx.la \
	$(top_builddir)/src/event/libadonthell_event.la \
	$(PY_RUNTIME)

## Rules to build libpyinput (libinput's Python wrapper)
_input_la_SOURCES = py_input_wrap.cc
_input_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_input_la_LDFLAGS = -avoid-version -module
_input_la_LIBADD =  $(PY_LIBADD) \
	 $(top_builddir)/src/base/libadonthell_base.la   \
	 $(top_builddir)/src/input/libadonthell_input.la \
	 $(PY_RUNTIME)

## Rules to build libpyaudio (libaudio's Python wrapper)
_audio_la_SOURCES = py_audio_wrap.cc
_audio_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_audio_la_LDFLAGS = -avoid-version -module
_audio_la_LIBADD =  $(PY_LIBADD) \
	$(top_builddir)/src/audio//libadonthell_audio.la \
	$(top_builddir)/src/event//libadonthell_event.la \
	$(PY_RUNTIME)

## Rules to build libpygui (libgui's Python wrapper)
_gui_la_SOURCES = py_gui_wrap.cc
_gui_la_CXXFLAGS = $(PY_CFLAGS) $(FT2_CFLAGS) $(AM_CXXFLAGS)
_gui_la_LDFLAGS = -avoid-version -module
_gui_la_LIBADD =  $(PY_LIBADD) \
	$(top_builddir)/src/base/libadonthell_base.la   \
	$(top_builddir)/src/gui//libadonthell_gui.la 	\
	$(top_builddir)/src/gfx/libadonthell_gfx.la	\
	$(top_builddir)/src/input/libadonthell_input.la	\
	$(top_builddir)/src/rpg/libadonthell_rpg.la	\
	$(PY_RUNTIME) $(FT2_LIBS)

## Rules to build libpymain (libmain's Python wrapper)
_main_la_SOURCES = py_main_wrap.cc
_main_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_main_la_LDFLAGS = -avoid-version -module
_main_la_LIBADD = $(PY_LIBADD) -ladonthell_py_main	\
	$(top_builddir)/src/main/libadonthell_main.la	\
	$(top_builddir)/src/gfx/libadonthell_gfx.la     \
	$(top_builddir)/src/input/libadonthell_input.la	\
	$(top_builddir)/src/audio/libadonthell_audio.la	\
	$(top_builddir)/src/event/libadonthell_event.la	\
	$(PY_RUNTIME)

## Rules to build libpyrpg (librpg's Python wrapper)
_rpg_la_SOURCES = py_rpg_wrap.cc
_rpg_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_rpg_la_LDFLAGS = -avoid-version -module
_rpg_la_LIBADD = $(PY_LIBADD)						\
	$(top_builddir)/src/rpg/libadonthell_rpg.la		\
	$(top_builddir)/src/event/libadonthell_event.la	\
	$(PY_RUNTIME)

## Rules to build libpyworld (libworlds's Python wrapper)
_world_la_SOURCES = py_world_wrap.cc
_world_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_world_la_LDFLAGS = -avoid-version -module
_world_la_LIBADD = $(PY_LIBADD)						\
	$(top_builddir)/src/world/libadonthell_world.la	\
    $(top_builddir)/src/base/libadonthell_base.la   \
    $(top_builddir)/src/gfx/libadonthell_gfx.la     \
	$(top_builddir)/src/event/libadonthell_event.la	\
	$(PY_RUNTIME)

## Rules to build libpydebug (helper for debugging the engine)
_debug_la_SOURCES = py_debug_wrap.cc
_debug_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
_debug_la_LDFLAGS = -avoid-version -module
_debug_la_LIBADD = $(PY_LIBADD) \
    $(top_builddir)/src/python/libadonthell_python.la \
	$(PY_RUNTIME)

## Swig arguments
SWIG_ARGS = -python -modern -noexcept -I$(top_srcdir)/src

## Rules for Python wrappers generation
base.py: py_base_wrap.cc
py_base_wrap.cc: py_base.i  $(top_srcdir)/src/base/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

event.py: py_event_wrap.cc
py_event_wrap.cc: py_event.i  $(top_srcdir)/src/event/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

gfx.py: py_gfx_wrap.cc
py_gfx_wrap.cc: py_gfx.i  $(top_srcdir)/src/gfx/*.h
	$(SWIG)  $(SWIG_ARGS) -o $(srcdir)/$@ $<

input.py: py_input_wrap.cc
py_input_wrap.cc: py_input.i $(top_srcdir)/src/input/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

audio.py: py_audio_wrap.cc
py_audio_wrap.cc: py_audio.i $(top_srcdir)/src/audio/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

gui.py: py_gui_wrap.cc
py_gui_wrap.cc: py_gui.i  $(top_srcdir)/src/gui/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

main.py: py_main_wrap.cc
py_main_wrap.cc: py_main.i  $(top_srcdir)/src/main/*.h
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

rpg.py: py_rpg_wrap.cc
py_rpg_wrap.cc: py_rpg.i  $(top_srcdir)/src/rpg/*.h
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

world.py: py_world_wrap.cc
py_world_wrap.cc: py_world.i  $(top_srcdir)/src/world/*.h
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

debug.py: py_debug_wrap.cc
py_debug_wrap.cc: py_debug.i $(top_srcdir)/src/py-runtime/py_runtime.i
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

__init__.py:
	touch $(srcdir)/$@

## The .pyc are generated so the Python modules are accessible even if
## we are using a separate builddir.
%.pyc: %.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$(srcdir)/$<\", \"./$@\");"

