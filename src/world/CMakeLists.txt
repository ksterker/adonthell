# Define the adonthell_world_SRCS variable containing all required files.
set(adonthell_world_SRCS
    area.cc
    character.cc
	character_with_gfx.cc
	coordinates.cc
	moving.cc
    object.cc
    object_with_gfx.cc
    placeable.cc
    placeable_area.cc
	placeable_area_gfx.cc
    placeable_gfx.cc
    placeable_model.cc
	placeable_model_gfx.cc
)

set(adonthell_world_HEADERS
    area.h
    character.h
	character_with_gfx.h
	coordinates.h
	moving.h
    object.h
    object_with_gfx.h
    placeable.h
    placeable_area.h
	placeable_area_gfx.h
    placeable_gfx.h
    placeable_model.h
	placeable_model_gfx.h
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
