# Define the adonthell_world_SRCS variable containing all required files.
set(adonthell_world_SRCS
    action.cc
    area.cc
    area_manager.cc
    character.cc
    collision.cc
    cube3.cc
    chunk.cc
    mapview.cc
    moving.cc
    object.cc
    placeable.cc
    placeable_model.cc
    placeable_shape.cc
    pathfinding.cc
    pathfinding_manager.cc
    plane3.cc
    renderer.cc
    schedule.cc
    schedule_data.cc
    shadow.cc
    triangle3.cc
    world.cc
)

set(adonthell_world_HEADERS
    action.h
    area.h
    area_manager.h
    character.h
    collision.h
    coordinates.h
    cube3.h
    chunk.h
    chunk_info.h
    entity.h
    mapview.h
    moving.h
	 node.h
	 node_cache.h
	 node_bank.h
    object.h
	 open_list.h
    placeable.h
    placeable_model.h
    placeable_shape.h
    plane3.h
	 pathfinding.h
	 pathfinding_manager.h
	 pathfinding_task.h
    render_info.h
    renderer.h
	 schedule.h
	 schedule_data.h
    shadow.h
    shadow_info.h
    triangle3.h
    vector3.h
    world.h
	 zone.h
)


# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_world SHARED ${adonthell_world_SRCS})
target_link_libraries(adonthell_world
	${PYTHON_LIBRARIES}
  ${LIBGLOG_LIBRARIES}
	adonthell_base
	adonthell_gfx
    adonthell_python
    adonthell_rpg
)

################################
# Unit tests
IF(DEVBUILD)
  add_executable(test_placeable test_placeable.cc)
  target_link_libraries(test_placeable ${TEST_LIBRARIES} adonthell_world)
  add_test(NAME WorldPlaceable COMMAND test_placeable)
ENDIF(DEVBUILD)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_world)
adonthell_install_include(world "${adonthell_world_HEADERS}") 
