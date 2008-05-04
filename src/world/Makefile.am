AM_CXXFLAGS = -I$(top_srcdir)/src
EXTRA_DIST = CMakeLists.txt

## Our header files
pkgincludeworlddir = $(pkgincludedir)/world
pkgincludeworld_HEADERS = \
    area.h \
    character.h \
	character_with_gfx.h \
    chunk.h \
    collision.h \
	coordinates.h \
	cube3.h \
	moving.h \
    object.h \
    object_with_gfx.h \
    placeable.h \
    placeable_gfx.h \
    placeable_model.h \
	placeable_model_gfx.h \
    placeable_shape.h \
    plane3.h \
	square.h \
    triangle3.h \
    vector3.h

## World library
lib_LTLIBRARIES = libadonthell_world.la

## Rules to build libworld
libadonthell_world_la_SOURCES = \
    area.cc \
    character.cc \
	character_with_gfx.cc \
    chunk.cc \
    collision.cc \
	coordinates.cc \
	cube3.cc \
	moving.cc \
    object.cc \
    object_with_gfx.cc \
    placeable.cc \
    placeable_gfx.cc \
    placeable_model.cc \
	placeable_model_gfx.cc \
    placeable_shape.cc \
    plane3.cc \
	square.cc \
    triangle3.cc \
	vector3.cc

libadonthell_world_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS) 
libadonthell_world_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base \
    -L../gfx -ladonthell_gfx $(SWIGRUNTIME_LIBS) -lstdc++
