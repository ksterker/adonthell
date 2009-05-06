AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir) $(FT2_CFLAGS)
EXTRA_DIST = CMakeLists.txt

pkgincludeinputdir = $(pkgincludedir)/gui
pkgincludeinput_HEADERS = \
	base.h \
	button.h \
	conversation.h \
	draw.h \
	font.h \
	gui.h \
	label.h \
	layout.h \
	option.h \
	textbox.h \
	widget.h \
    window_manager.h

lib_LTLIBRARIES = libadonthell_gui.la

libadonthell_gui_la_SOURCES = \
	button.cpp \
	conversation.cc \
	font.cpp \
	label.cpp \
	layout.cc \
	option.cpp \
	textbox.cpp \
	widget.cpp \
    window_manager.cc

libadonthell_gui_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" -DDEBUG $(AM_CXXFLAGS) $(PY_CFLAGS) -D__BIG_ENDIAN__

libadonthell_gui_la_LIBADD = -L$(top_builddir)/src/base/ -ladonthell_base \
	-L$(top_builddir)/src/input/ -ladonthell_input \
	-L$(top_builddir)/src/gfx/ -ladonthell_gfx \
	-lstdc++ $(FT2_LIBS) -L$(top_builddir)/src/python/ -ladonthell_python $(PY_LIBS) -L$(top_builddir)/src/rpg/ -ladonthell_rpg 
