CXXFLAGS += -I$(top_srcdir)/src

## Our header files
pkgincludeworlddir = $(pkgincludedir)/world
pkgincludeworld_HEADERS = \
    area.h \
    character.h \
	character_with_gfx.h \
	coordinates.h \
	moving.h \
    object.h \
    object_with_gfx.h \
    placeable.h \
    placeable_area.h \
	placeable_area_gfx.h \
    placeable_gfx.h \
    placeable_model.h \
	placeable_model_gfx.h

## World library
lib_LTLIBRARIES = libadonthell_world.la

## Rules to build librpg
libadonthell_world_la_SOURCES = \
    area.cc \
    character.cc \
	character_with_gfx.cc \
	coordinates.cc \
	moving.cc \
    object.cc \
    object_with_gfx.cc \
    placeable.cc \
    placeable_area.cc \
	placeable_area_gfx.cc \
    placeable_gfx.cc \
    placeable_model.cc \
	placeable_model_gfx.cc

libadonthell_world_la_CXXFLAGS = $(PY_CFLAGS)
libadonthell_world_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base \
    -L../gfx -ladonthell_gfx $(SWIGRUNTIME_LIBS) -lstdc++

