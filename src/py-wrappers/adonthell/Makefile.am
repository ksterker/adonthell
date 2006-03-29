CXXFLAGS += -I$(top_srcdir)/src/
EXTRA_DIST = py_base.i py_event.i py_gfx.i py_input.i py_audio.i py_gui.i py_main.i py_runtime.i py_debug.i
CLEANFILES = $(top_srcdir)/src/py-wrappers/adonthell/py_*_wrap.cc

## Python wrapper files.
pkgpyexec_PYTHON = __init__.py base.py event.py gfx.py input.py audio.py gui.py main.py rpg.py debug.py
noinst_SCRIPTS = __init__.pyc base.pyc event.pyc gfx.pyc input.pyc audio.pyc gui.pyc main.pyc rpg.pyc debug.pyc

## Python helper libraries
pkgpyexec_LTLIBRARIES = _base.la _event.la _gfx.la _input.la _audio.la _gui.la _main.la _rpg.la _debug.la

## SWIG runtime support
lib_LTLIBRARIES = libadonthell_py_main.la libadonthell_py_runtime.la

## the swig runtime code
py_runtime.cc: py_runtime.i Makefile.am
	$(SWIG) -c++ -noexcept -nocontract -python -modern -module py_runtime -o $(srcdir)/$@ $<

## Rules to build libadonthell_py_main
libadonthell_py_main_la_SOURCES = py_main.cc
libadonthell_py_main_la_CXXFLAGS = $(PY_CFLAGS)
libadonthell_py_main_la_LIBADD = $(PY_LIBS)

## Rules to build libadonthell_py_runtime
libadonthell_py_runtime_la_SOURCES = py_runtime.cc
libadonthell_py_runtime_la_CXXFLAGS = $(PY_CFLAGS) -DSWIGRUNTIME= -DSWIGINLINE= 
libadonthell_py_runtime_la_LIBADD = $(PY_LIBS)

PY_LIBADD = -L$(top_builddir)/src/python -ladonthell_python $(PY_LIBS)

## Rules to build libpybase (libbase's Python wrapper)
_base_la_SOURCES = py_base_wrap.cc
_base_la_CXXFLAGS = $(PY_CFLAGS)
_base_la_LDFLAGS = -avoid-version -module
_base_la_LIBADD = $(PY_LIBADD) -L$(top_builddir)/src/base/ -ladonthell_base -ladonthell_py_runtime

## Rules to build libpyevent (libevent's Python wrapper)
_event_la_SOURCES = py_event_wrap.cc
_event_la_CXXFLAGS = $(PY_CFLAGS)
_event_la_LDFLAGS = -avoid-version -module
_event_la_LIBADD = $(PY_LIBADD) -L$(top_builddir)/src/event/ -ladonthell_event -ladonthell_py_runtime

## Rules to build libpygfx (libgfx's Python wrapper)
_gfx_la_SOURCES = py_gfx_wrap.cc
_gfx_la_CXXFLAGS = $(PY_CFLAGS)
_gfx_la_LDFLAGS = -avoid-version -module
_gfx_la_LIBADD = $(PY_LIBADD) -L$(top_builddir)/src/gfx/ -ladonthell_gfx -ladonthell_py_runtime

## Rules to build libpyinput (libinput's Python wrapper)
_input_la_SOURCES = py_input_wrap.cc
_input_la_CXXFLAGS = $(PY_CFLAGS)
_input_la_LDFLAGS = -avoid-version -module
_input_la_LIBADD =  $(PY_LIBADD) -L$(top_builddir)/src/input/ -ladonthell_input -ladonthell_py_runtime

## Rules to build libpyaudio (libaudio's Python wrapper)
_audio_la_SOURCES = py_audio_wrap.cc
_audio_la_CXXFLAGS = $(PY_CFLAGS)
_audio_la_LDFLAGS = -avoid-version -module
_audio_la_LIBADD =  $(PY_LIBADD) -L$(top_builddir)/src/audio/ -ladonthell_audio -ladonthell_py_runtime

## Rules to build libpygui (libgui's Python wrapper)
_gui_la_SOURCES = py_gui_wrap.cc
_gui_la_CXXFLAGS = $(PY_CFLAGS)
_gui_la_LDFLAGS = -avoid-version -module
_gui_la_LIBADD =  $(PY_LIBADD) -L$(top_builddir)/src/gui/ -ladonthell_gui -ladonthell_py_runtime

## Rules to build libpymain (libmain's Python wrapper)
_main_la_SOURCES = py_main_wrap.cc
_main_la_CXXFLAGS = $(PY_CFLAGS)
_main_la_LDFLAGS = -avoid-version -module
_main_la_LIBADD = $(PY_LIBADD) -ladonthell_py_main	\
	-L$(top_builddir)/src/main -ladonthell_main		\
	-L$(top_builddir)/src/gfx -ladonthell_gfx		\
	-L$(top_builddir)/src/input -ladonthell_input	\
	-L$(top_builddir)/src/audio -ladonthell_audio	\
	-L$(top_builddir)/src/event -ladonthell_event	\
	-ladonthell_py_runtime

## Rules to build libpyrpg (librpg's Python wrapper)
_rpg_la_SOURCES = py_rpg_wrap.cc
_rpg_la_CXXFLAGS = $(PY_CFLAGS)
_rpg_la_LDFLAGS = -avoid-version -module
_rpg_la_LIBADD = $(PY_LIBADD)						\
	-L$(top_builddir)/src/rpg -ladonthell_rpg		\
	-L$(top_builddir)/src/event -ladonthell_event	\
	-ladonthell_py_runtime

## Rules to build libpydebug (helper for debugging the engine)
_debug_la_SOURCES = py_debug_wrap.cc
_debug_la_CXXFLAGS = $(PY_CFLAGS)
_debug_la_LDFLAGS = -avoid-version -module
_debug_la_LIBADD = $(PY_LIBADD) -L$(top_builddir)/src/python -ladonthell_python -ladonthell_py_runtime


## Swig arguments
SWIG_ARGS = -python -modern -noexcept -I$(top_srcdir)/src

## Rules for Python wrappers generation
## The .pyc are generated so the Python modules are accessible even if
## we are using a separate builddir.
base.pyc: base.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
base.py: py_base_wrap.cc
py_base_wrap.cc: py_base.i  $(top_srcdir)/src/base/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

event.pyc: event.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
event.py: py_event_wrap.cc
py_event_wrap.cc: py_event.i  $(top_srcdir)/src/event/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

gfx.pyc: gfx.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
gfx.py: py_gfx_wrap.cc
py_gfx_wrap.cc: py_gfx.i  $(top_srcdir)/src/gfx/*.h
	$(SWIG)  $(SWIG_ARGS) -o $(srcdir)/$@ $<

input.pyc: input.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
input.py: py_input_wrap.cc
py_input_wrap.cc: py_input.i $(top_srcdir)/src/input/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

audio.pyc: audio.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
audio.py: py_audio_wrap.cc
py_audio_wrap.cc: py_audio.i $(top_srcdir)/src/audio/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

gui.pyc: gui.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
gui.py: py_gui_wrap.cc
py_gui_wrap.cc: py_gui.i  $(top_srcdir)/src/gui/*.h
	$(SWIG) $(SWIG_ARGS) -o $(srcdir)/$@ $<

main.pyc: main.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
main.py: py_main_wrap.cc
py_main_wrap.cc: py_main.i  $(top_srcdir)/src/main/*.h
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

rpg.pyc: rpg.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
rpg.py: py_rpg_wrap.cc
py_rpg_wrap.cc: py_rpg.i  $(top_srcdir)/src/rpg/*.h
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

debug.pyc: debug.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$<\", \"./$@\");"
debug.py: py_debug_wrap.cc
py_debug_wrap.cc: py_debug.i py_runtime.i
	$(SWIG) $(SWIG_ARGS) -I$(srcdir)/$@ -o $(srcdir)/$@ $<

__init__.pyc: __init__.py
	$(PYTHON) -c "import py_compile; py_compile.compile(\"$(srcdir)/$<\", \"./$@\");"
__init__.py:
	touch $(srcdir)/$@
