CXXFLAGS += -I$(top_srcdir)/src/

EXTRA_DIST =  configtest.py eventtest.py filereport.py gfxtest.py guitest.py inputtest.py
noinst_PROGRAMS = callbacktest diskiotest guitest inputtest

callbacktest_SOURCES = callbacktest.cc
callbacktest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base

diskiotest_SOURCES = diskiotest.cc
diskiotest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base

guitest_SOURCES = guitest.cc
guitest_LDADD = \
	-L$(top_builddir)/src/gui/ -ladonthell_gui                 \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                 \
	-L$(top_builddir)/src/input/ -ladonthell_input             \
	-L$(top_builddir)/src/event/ -ladonthell_event             \
	-L$(top_builddir)/src/py-wrappers/adonthell/ -lswigruntime \
	-L$(top_builddir)/src/base/ -ladonthell_base               \
	-L$(top_builddir)/src/main -ladonthell_main                \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS)

inputtest_SOURCES = inputtest.cc
inputtest_LDADD = \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx                  \
	-L$(top_builddir)/src/input/ -ladonthell_input              \
	-L$(top_builddir)/src/event/ -ladonthell_event              \
	-L$(top_builddir)/src/py-wrappers/adonthell/ -lswigruntime  \
	-L$(top_builddir)/src/base/ -ladonthell_base                \
	-L$(top_builddir)/src/main -ladonthell_main                 \
	-L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS)
