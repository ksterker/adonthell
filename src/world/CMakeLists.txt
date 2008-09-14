# Define the adonthell_world_SRCS variable containing all required files.
set(adonthell_world_SRCS
    area.cc
    character.cc
    cube3.cc
    chunk.cc
    mapview.cc
    moving.cc
    object.cc
    placeable.cc
    placeable_model.cc
    placeable_shape.cc
    plane3.cc
    square.cc
    triangle3.cc
)

set(adonthell_world_HEADERS
    area.h
    character.h
    collision.h
    coordinates.h
    cube3.h
    chunk.h
    entity.h
    mapview.h
    moving.h
    object.h
    placeable.h
    placeable_model.h
    placeable_shape.h
    plane3.h
    render_info.h
    square.h
    triangle3.h
    vector3.h
)


# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_world SHARED ${adonthell_world_SRCS})
target_link_libraries(adonthell_world
	${PYTHON_LIBRARIES}
	adonthell_base
	adonthell_gfx
)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_world)
adonthell_install_include(world "${adonthell_world_HEADERS}") 
