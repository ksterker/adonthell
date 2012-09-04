# -- this order is important, as we build with -no-undefined
SUBDIRS = . base python py-runtime event gfx input audio rpg world gui main py-wrappers

EXTRA_DIST = CMakeLists.txt

noinst_SCRIPTS = $(top_builddir)/src/run.sh $(top_builddir)/src/python.sh

## Script to run binaries while the libraries aren't installed
$(top_builddir)/src/run.sh:
	echo -e "#/bin/sh\nADONTHELL_MODULES_PATH=\"`pwd`\" \$$$$@@" > $@

## Script to run Python programs while the libraries aren't installed
$(top_builddir)/src/python.sh:
	echo -e "#/bin/sh\nPYTHONPATH=\"$$PYTHONPATH:`pwd`/py-wrappers:`pwd`/py-wrappers/adonthell/.libs\" source `pwd`/run.sh python \$$$$@@" > $@

if BUILD_GTEST

noinst_LIBRARIES = libgtest.a
libgtest_a_SOURCES =  $(GTEST_DIR)/src/gtest-all.cc
libgtest_a_CXXFLAGS = -I$(GTEST_DIR)

endif

if BUILD_GMOCK

noinst_LIBRARIES = libgmock.a

libgmock_a_SOURCES = $(GMOCK_DIR)/src/gmock-all.cc
libgmock_a_CXXFLAGS = $(libgtest_CFLAGS)
libgmock_a_LIBADD = $(libgtest_LIBS)

endif
	