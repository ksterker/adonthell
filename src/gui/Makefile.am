CXXFLAGS += -I$(top_srcdir)/src/
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir)

pkgincludeinputdir = $(pkgincludedir)/gui
pkgincludeinput_HEADERS = \
	base.h \
	container.h \
	layout.h listlayout.h

lib_LTLIBRARIES = libadonthell_gui.la

libadonthell_gui_la_SOURCES = \
	base.cc \
	container.cc \
	layout.cc listlayout.cc

libadonthell_gui_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\"

libadonthell_gui_la_LIBADD = -L$(top_builddir)/src/base/ -ladonthell_base \
	-L$(top_builddir)/src/input/ -ladonthell_input \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx \
	-lstdc++
