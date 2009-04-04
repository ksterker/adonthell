AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir) $(FT2_CFLAGS)
EXTRA_DIST = CMakeLists.txt

pkgincludeinputdir = $(pkgincludedir)/gui
pkgincludeinput_HEADERS = \
	base.h \
	button.h \
	draw.h \
	font.h \
	gui.h \
	label.h \
	layout.h \
	option.h \
	widget.h \
	conversation.h

lib_LTLIBRARIES = libadonthell_gui.la

libadonthell_gui_la_SOURCES = \
	font.cpp \
	widget.cpp \
	label.cpp \
	button.cpp \
	layout.cc \
	option.cpp \
	textbox.cpp \
	conversation.cc

libadonthell_gui_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" -DDEBUG $(AM_CXXFLAGS) $(PY_CFLAGS)

libadonthell_gui_la_LIBADD = -L$(top_builddir)/src/base/ -ladonthell_base \
	-L$(top_builddir)/src/input/ -ladonthell_input \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx \
	-lstdc++ $(FT2_LIBS) -ladonthell_python $(PY_LIBS) -ladonthell_rpg 
