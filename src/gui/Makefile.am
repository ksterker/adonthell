AM_CXXFLAGS = -I$(top_srcdir)/src
DEFAULT_INCLUDES = -I$(srcdir) -I$(top_builddir) $(FT2_CFLAGS)
EXTRA_DIST = CMakeLists.txt

pkgincludeinputdir = $(pkgincludedir)/gui
pkgincludeinput_HEADERS = \
	button.h \
    conversation.h \
    decoration.h \
	draw.h \
	font.h \
	gui.h \
	label.h \
	layout.h \
	option.h \
	scrollview.h \
	textbox.h \
    textcache.h \
	widget.h \
    window_manager.h

lib_LTLIBRARIES = libadonthell_gui.la

libadonthell_gui_la_SOURCES = \
	button.cpp \
    conversation.cc \
    decoration.cc \
	font.cpp \
	label.cpp \
	layout.cc \
	option.cpp \
    scrollview.cc \
	textbox.cpp \
    textcache.cc \
	widget.cpp \
    window_manager.cc

libadonthell_gui_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" -DDEBUG $(AM_CXXFLAGS) $(PY_CFLAGS) -D__BIG_ENDIAN__

libadonthell_gui_la_LIBADD = $(PY_LIBS) \
    $(top_builddir)/src/base/libadonthell_base.la \
	$(top_builddir)/src/input/libadonthell_input.la \
	$(top_builddir)/src/gfx/libadonthell_gfx.la \
    $(top_builddir)/src/python/libadonthell_python.la \
    $(top_builddir)/src/rpg/libadonthell_rpg.la \
	-lstdc++ $(FT2_LIBS)
