SUBDIRS = main base event gfx input audio gui python rpg world py-wrappers

noinst_SCRIPTS = $(top_builddir)/src/run.sh $(top_builddir)/src/python.sh

## Script to run binaries while the libraries aren't installed
$(top_builddir)/src/run.sh:
	echo -e "#/bin/sh\nADONTHELL_MODULES_PATH=\"`pwd`\" \$$$$@@" > $@

## Script to run Python programs while the libraries aren't installed
$(top_builddir)/src/python.sh:
	echo -e "#/bin/sh\nPYTHONPATH=\"$$PYTHONPATH:`pwd`/py-wrappers:`pwd`/py-wrappers/adonthell/.libs\" source `pwd`/run.sh python \$$$$@@" > $@

