CXXFLAGS += -I$(top_srcdir)/src/

## Our header files
pkgincluderpgdir = $(pkgincludedir)/rpg
pkgincluderpg_HEADERS = \
    character.h \
    equipment.h \
    inventory.h \
    item_storage.h \
    item.h \
    slot.h
    
## Rpg library
lib_LTLIBRARIES = libadonthell_rpg.la

## Rules to build librpg
libadonthell_rpg_la_SOURCES = \
    character.cc \
    equipment.cc \
    inventory.cc \
    item_storage.cc \
    item.cc \
    slot.cc

libadonthell_rpg_la_CXXFLAGS = $(PY_CFLAGS)
libadonthell_rpg_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base $(SWIGRUNTIME_LIBS) -lstdc++
