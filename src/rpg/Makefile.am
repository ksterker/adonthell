AM_CXXFLAGS = -I$(top_srcdir)/src
EXTRA_DIST = CMakeLists.txt

## Our header files
pkgincluderpgdir = $(pkgincludedir)/rpg
pkgincluderpg_HEADERS = \
    character.h \
    dialog.h \
    dialog_line.h \
    equipment.h \
    inventory.h \
    item_storage.h \
    item.h \
    log_entry.h \
    log_index.h \
    quest.h \
	quest_event.h \
	quest_event_manager.h \
    schedule.h \
    schedule_data.h \
    slot.h
    
## Rpg library
lib_LTLIBRARIES = libadonthell_rpg.la

## Rules to build librpg
libadonthell_rpg_la_SOURCES = \
    character.cc \
    dialog.cc \
    dialog_line.cc \
    equipment.cc \
    inventory.cc \
    item_storage.cc \
    item.cc \
    log_entry.cc \
    log_index.cc \
    quest.cc \
	quest_event.cc \
	quest_event_manager.cc \
    schedule.cc\
    schedule_data.cc \
    slot.cc

libadonthell_rpg_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_rpg_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base $(SWIGRUNTIME_LIBS) -lstdc++
