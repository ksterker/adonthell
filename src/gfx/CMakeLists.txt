# Define the adonthell_gfx_SRCS variable containing all required files.
set(adonthell_gfx_SRCS
	drawable.cc
	drawing_area.cc
	png_wrapper.cc
	gfx.cc
    image.cc
	screen.cc
    sprite.cc
	surface.cc
    surface_cacher.cc
	surface_ext.cc
)

set(adonthell_gfx_HEADERS
	drawable.h
	drawing_area.h
	gfx.h
    image.h
	png_wrapper.h
	screen.h
    sprite.h
	surface.h
    surface_cacher.h
	surface_ext.h
)

add_definitions(${PNG_DEFINITIONS})

include_directories(${PYTHON_INCLUDE_PATH} ${PNG_INCLUDE_DIR})

# Create a shared library
add_library(adonthell_gfx SHARED ${adonthell_gfx_SRCS})
target_link_libraries(adonthell_gfx
	${PYTHON_LIBRARIES}
	adonthell_base
	adonthell_event
	${PNG_LIBRARY}
	)


################################
# Create the SDL Backend
IF(${SDL_VERSION} STREQUAL 2.0)
set(gfx_sdl_SRCS
	sdl2/gfx_sdl.cc
    sdl2/screen_sdl.cc
    sdl2/surface_sdl.cc
)
ELSE(${SDL_VERSION} STREQUAL 2.0)
set(gfx_sdl_SRCS
	sdl/gfx_sdl.cc
    sdl/screen_sdl.cc
    sdl/surface_sdl.cc
)
ENDIF(${SDL_VERSION} STREQUAL 2.0)

# Add specific include directory for this library.
include_directories(${SDL_INCLUDE_DIR})

add_library(gfx-backend-sdl MODULE ${gfx_sdl_SRCS})
target_link_libraries(gfx-backend-sdl
	${SDL_LIBRARY}
	adonthell_gfx
	)
set_target_properties(gfx-backend-sdl PROPERTIES PREFIX "_" OUTPUT_NAME "sdl")

###########################################################
# Create the OpenGL Backend if OpenGL and SDL are available

#############################################
# Install Stuff
adonthell_install_lib(adonthell_gfx)
adonthell_install_include(gfx "${adonthell_gfx_HEADERS}")
adonthell_install_backend(gfx gfx-backend-sdl)
