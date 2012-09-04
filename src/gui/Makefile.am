AM_CXXFLAGS = -I$(top_builddir)
DEFAULT_INCLUDES = -I$(top_builddir) $(FT2_CFLAGS)
EXTRA_DIST = CMakeLists.txt

adonthellincludedir = $(pkgincludedir)-@VERSION@/adonthell

pkgincludeguidir = $(adonthellincludedir)/gui
pkgincludegui_HEADERS = \
	button.h \
	canvas.h \
    conversation.h \
    decoration.h \
	font.h \
	fontcache.h \
	gui.h \
	indicatorbar.h \
	label.h \
	layout.h \
	listlayout.h \
	option.h \
	scrollview.h \
	textbox.h \
	ui_event.h \
	ui_event_manager.h \
	widget.h \
    window_manager.h

lib_LTLIBRARIES = libadonthell_gui.la

libadonthell_gui_la_SOURCES = \
	button.cpp \
	canvas.cc \
    conversation.cc \
    decoration.cc \
	font.cpp \
    fontcache.cc \
	label.cpp \
	layout.cc \
    listlayout.cc \
	indicatorbar.cc \
	option.cpp \
    scrollview.cc \
	textbox.cpp \
	ui_event.cc \
	ui_event_manager.cc \
	widget.cpp \
    window_manager.cc

libadonthell_gui_la_CXXFLAGS = -DPKGLIBDIR=\"$(pkglibdir)\" -DDEBUG $(AM_CXXFLAGS) $(PY_CFLAGS)

libadonthell_gui_la_LIBADD = $(PY_LIBS) \
    $(top_builddir)/src/base/libadonthell_base.la \
    $(top_builddir)/src/event/libadonthell_event.la \
	$(top_builddir)/src/input/libadonthell_input.la \
	$(top_builddir)/src/gfx/libadonthell_gfx.la \
    $(top_builddir)/src/python/libadonthell_python.la \
    $(top_builddir)/src/rpg/libadonthell_rpg.la \
    $(top_builddir)/src/world/libadonthell_world.la \
	-lstdc++ $(FT2_LIBS)

## Unit tests
test_CXXFLAGS = $(libgmock_CFLAGS) $(libgtest_CFLAGS)
test_LDADD    = $(libgmock_LIBS)   $(libgtest_LIBS) $(top_builddir)/src/gui/libadonthell_gui.la

test_ui_event_manager_SOURCES  = test_ui_event_manager.cc
test_ui_event_manager_CXXFLAGS = $(libadonthell_gui_la_CXXFLAGS) $(test_CXXFLAGS)
test_ui_event_manager_LDADD    = $(libadonthell_gui_la_LIBADD)   $(test_LDADD)

TESTS = \
	test_ui_event_manager

check_PROGRAMS = $(TESTS)
