SUBDIRS = runtime adonthell
EXTRA_DIST = CMakeLists.txt

install-data-hook:
if PY_FRAMEWORK
	cp -r $(DESTDIR)$(PY_SPDIR)/adonthell $(DESTDIR)$(PY_FWDIR)
endif
