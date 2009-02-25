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
IF(${SDL_VERSION} STREQUAL 1.3)
set(gfx_sdl_SRCS
	sdl13/gfx_sdl.cc
    sdl13/screen_sdl.cc
    sdl13/surface_sdl.cc
)
ELSE(${SDL_VERSION} STREQUAL 1.3)
set(gfx_sdl_SRCS
	sdl/gfx_sdl.cc
    sdl/screen_sdl.cc
    sdl/surface_sdl.cc
)
ENDIF(${SDL_VERSION} STREQUAL 1.3)

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
IF(OPENGL_FOUND)
set(gfx_sdlgl_SRCS
	sdlgl/gfx_sdlgl.cc
        sdlgl/screen_sdlgl.cc
        sdlgl/surface_sdlgl.cc
)

# Add specific include directory for this library.
include_directories(${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_library(gfx-backend-sdlgl MODULE ${gfx_sdlgl_SRCS})
target_link_libraries(gfx-backend-sdlgl
	${SDL_LIBRARY}
	${OPENGL_LIBRARY}
	adonthell_gfx
	)
set_target_properties(gfx-backend-sdlgl PROPERTIES PREFIX "_" OUTPUT_NAME "sdlgl")
ENDIF(OPENGL_FOUND)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_gfx)
adonthell_install_include(gfx "${adonthell_gfx_HEADERS}")
adonthell_install_backend(gfx gfx-backend-sdl)

IF(OPENGL_FOUND)
adonthell_install_backend(gfx gfx-backend-sdlgl)
ENDIF(OPENGL_FOUND)
