CXXFLAGS += -I$(top_srcdir)/src

EXTRA_DIST = README audiotest.py dialogtest.py gfxsave.py itemtest.py \
    worldtest.py configtest.py equipmenttest.py  gfxtest.py logtest.py \
    convert_data.py eventtest.py guitest.py questtest.py \
    convert_quests.py filereport.py inputtest.py searchtest.py \
    convert_graphics.py

noinst_PROGRAMS = callbacktest diskiotest guitest inputtest gfxtest

callbacktest_SOURCES = callbacktest.cc
callbacktest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base

diskiotest_SOURCES = diskiotest.cc
diskiotest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base

guitest_SOURCES = guitest.cc
guitest_LDADD = \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS)\
	-L$(top_builddir)/src/gui/ -ladonthell_gui                 \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                 \
	-L$(top_builddir)/src/input/ -ladonthell_input             \
	-L$(top_builddir)/src/audio/ -ladonthell_audio              \
	-L$(top_builddir)/src/event/ -ladonthell_event             \
	-L$(top_builddir)/src/base/ -ladonthell_base               \
	-L$(top_builddir)/src/main -ladonthell_main                \
	-L$(top_builddir)/src/py-wrappers/adonthell -ladonthell_py_runtime

inputtest_SOURCES = inputtest.cc

inputtest_LDADD = \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                  \
	-L$(top_builddir)/src/input/ -ladonthell_input              \
	-L$(top_builddir)/src/audio/ -ladonthell_audio              \
	-L$(top_builddir)/src/event/ -ladonthell_event              \
	-L$(top_builddir)/src/base/ -ladonthell_base                \
	-L$(top_builddir)/src/main -ladonthell_main                 \
	-L$(top_builddir)/src/py-wrappers/adonthell -ladonthell_py_runtime

gfxtest_SOURCES = gfxtest.cc
gfxtest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx
