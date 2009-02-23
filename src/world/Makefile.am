AM_CXXFLAGS = -I$(top_srcdir)/src
EXTRA_DIST = CMakeLists.txt

## Our header files
pkgincludeworlddir = $(pkgincludedir)/world
pkgincludeworld_HEADERS = \
    area.h \
    character.h \
    chunk.h \
    chunk_info.h \
    collision.h \
	 coordinates.h \
	 cube3.h \
    entity.h \
    mapview.h \
	 moving.h \
	 node_bank.h \
	 node_cache.h \
	 node.h \
    object.h \
	 open_list.h \
    placeable.h \
    placeable_model.h \
    placeable_shape.h \
    plane3.h \
	 pathfinding.h \
    render_info.h \
    renderer.h \
    shadow.h \
    shadow_info.h \
    triangle3.h \
    vector3.h

## World library
lib_LTLIBRARIES = libadonthell_world.la

## Rules to build libworld
libadonthell_world_la_SOURCES = \
    area.cc \
    character.cc \
    chunk.cc \
    collision.cc \
	 cube3.cc \
    mapview.cc \
	 moving.cc \
    object.cc \
    placeable.cc \
    placeable_model.cc \
    placeable_shape.cc \
    plane3.cc \
	 pathfinding.cc \
    renderer.cc \
    shadow.cc \
    triangle3.cc

libadonthell_world_la_CXXFLAGS = $(PY_CFLAGS) $(AM_CXXFLAGS)
libadonthell_world_la_LIBADD = $(PY_LIBS) -L../base -ladonthell_base \
    -L../gfx -ladonthell_gfx $(SWIGRUNTIME_LIBS) -lstdc++
