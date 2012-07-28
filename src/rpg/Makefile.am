AM_CXXFLAGS = -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

adonthellincludedir = $(pkgincludedir)-@VERSION@/adonthell

## Our header files
pkgincluderpgdir = $(adonthellincludedir)/rpg
pkgincluderpg_HEADERS = \
    character.h \
    dialog.h \
    dialog_line.h \
    equipment.h \
    faction.h \
    inventory.h \
    item_storage.h \
    item.h \
    log_entry.h \
    log_index.h \
    pathfinding_costs.h \
    quest.h \
    quest_event.h \
    quest_event_manager.h \
    specie.h \
    group.h \
    slot.h
    
## Rpg library
lib_LTLIBRARIES = libadonthell_rpg.la

## Rules to build librpg
libadonthell_rpg_la_SOURCES = \
    character.cc \
    dialog.cc \
    dialog_line.cc \
    equipment.cc \
    faction.cc \
    inventory.cc \
    item_storage.cc \
    item.cc \
    log_entry.cc \
    log_index.cc \
    pathfinding_costs.cc \
    quest.cc \
    quest_event.cc \
    quest_event_manager.cc \
    specie.cc \
    group.cc \
    slot.cc

libadonthell_rpg_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_rpg_la_LIBADD = $(PY_LIBS) \
    $(top_builddir)/src/base/libadonthell_base.la \
    $(top_builddir)/src/event/libadonthell_event.la \
    $(top_builddir)/src/python/libadonthell_python.la \
    $(top_builddir)/src/py-runtime/libadonthell_py_runtime.la \
    -lstdc++

