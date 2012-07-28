AM_CXXFLAGS = -I$(top_builddir)
EXTRA_DIST = CMakeLists.txt

adonthellincludedir = $(pkgincludedir)-@VERSION@/adonthell

## Our header files
pkgincludeworlddir = $(adonthellincludedir)/world
pkgincludeworld_HEADERS = \
    action.h \
    area.h \
    area_manager.h \
    character.h \
    chunk.h \
    chunk_info.h \
    collision.h \
    coordinates.h \
    cube3.h \
    entity.h \
    mapview.h \
    move_event.h \
    move_event_manager.h \
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
    pathfinding_manager.h \
    pathfinding_task.h \
    render_info.h \
    renderer.h \
    schedule.h \
    schedule_data.h \
    shadow.h \
    shadow_info.h \
    triangle3.h \
    vector3.h \
    world.h \
    zone.h

## World library
lib_LTLIBRARIES = libadonthell_world.la

## Rules to build libworld
libadonthell_world_la_SOURCES = \
    action.cc \
    area.cc \
    area_manager.cc \
    character.cc \
    chunk.cc \
    chunk_info.cc \
    collision.cc \
    cube3.cc \
    mapview.cc \
    move_event.cc \
    move_event_manager.cc \
    moving.cc \
    object.cc \
    placeable.cc \
    placeable_model.cc \
    placeable_shape.cc \
    plane3.cc \
    pathfinding.cc \
    pathfinding_manager.cc \
    renderer.cc \
    schedule.cc \
    schedule_data.cc \
    shadow.cc \
    triangle3.cc \
    world.cc

libadonthell_world_la_CXXFLAGS = $(PY_CFLAGS) $(libglog_CFLAGS) $(AM_CXXFLAGS)
libadonthell_world_la_LIBADD = $(PY_LIBS) $(libglog_LIBS) \
    $(top_builddir)/src/base/libadonthell_base.la \
    $(top_builddir)/src/python/libadonthell_python.la \
    $(top_builddir)/src/event/libadonthell_event.la \
    $(top_builddir)/src/gfx/libadonthell_gfx.la \
    $(top_builddir)/src/rpg/libadonthell_rpg.la \
    $(top_builddir)/src/py-runtime/libadonthell_py_runtime.la -lstdc++


## Unit tests
test_CXXFLAGS = $(libgmock_CFLAGS) $(libgtest_CFLAGS)
test_LDADD    = $(libgmock_LIBS)   $(libgtest_LIBS) $(top_builddir)/src/world/libadonthell_world.la

test_renderer_SOURCES  = test_renderer.cc
test_renderer_CXXFLAGS = $(libadonthell_world_la_CXXFLAGS) $(test_CXXFLAGS)
test_renderer_LDADD    = $(libadonthell_world_la_LIBADD)   $(test_LDADD)

TESTS = \
	test_renderer

check_PROGRAMS = $(TESTS)
