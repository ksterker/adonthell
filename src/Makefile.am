# -- this order is important, as we build with -no-undefined
SUBDIRS = base python py-runtime event gfx input audio rpg gui world main py-wrappers test

EXTRA_DIST = CMakeLists.txt

noinst_SCRIPTS = $(top_builddir)/src/run.sh $(top_builddir)/src/python.sh

## Script to run binaries while the libraries aren't installed
$(top_builddir)/src/run.sh:
	echo -e "#/bin/sh\nADONTHELL_MODULES_PATH=\"`pwd`\" \$$$$@@" > $@

## Script to run Python programs while the libraries aren't installed
$(top_builddir)/src/python.sh:
	echo -e "#/bin/sh\nPYTHONPATH=\"$$PYTHONPATH:`pwd`/py-wrappers:`pwd`/py-wrappers/adonthell/.libs\" source `pwd`/run.sh python \$$$$@@" > $@

