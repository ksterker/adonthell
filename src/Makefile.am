SUBDIRS = main base event gfx input gui python py-wrappers

noinst_SCRIPTS = run.sh python.sh

## Script to run binaries while the libraries aren't installed
run.sh:
	echo -e "#/bin/sh\nADONTHELL_MODULES_PATH=\"`pwd`\" \$$$$@@" > $@

## Script to run Python programs while the libraries aren't installed
python.sh:
	echo -e "#/bin/sh\nPYTHONPATH=\"$$PYTHONPATH:`pwd`/$(top_srcdir)/src/py-wrappers:`pwd`/py-wrappers/adonthell/.libs\" source `pwd`/run.sh python \$$$$@@" > $@
