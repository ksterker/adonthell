SUBDIRS = runtime adonthell
EXTRA_DIST = CMakeLists.txt

if PY_FRAMEWORK
install-data-hook:
	cp -r $(DESTDIR)$(PY_SPDIR)/adonthell $(DESTDIR)$(PY_FWDIR)
endif
