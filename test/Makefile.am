CXXFLAGS += -I$(top_srcdir)/src/

EXTRA_DIST = gfxtest.py inputtest.py guitest.py
noinst_PROGRAMS = callbacktest inputtest

callbacktest_SOURCES = callbacktest.cc
callbacktest_LDADD = -L$(top_builddir)/src/base/ -ladonthell_base

inputtest_SOURCES = inputtest.cc
inputtest_LDADD = -L$(top_builddir)/src/gfx/ -ladonthell_gfx -L$(top_builddir)/src/input/ -ladonthell_input -L$(top_builddir)/src/main -ladonthell_main
