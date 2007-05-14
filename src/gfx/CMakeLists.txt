# Define the adonthell_gfx_SRCS variable containing all required files.
set(adonthell_gfx_SRCS
	gfx.cc
	drawing_area.cc
	drawable.cc
	png_wrapper.cc
	surface.cc
	screen.cc
	animation.cc
)

set(adonthell_gfx_HEADERS
	animation.h
	drawing_area.h
	png_wrapper.h
	surface.h
	drawable.h
	gfx.h
	screen.h
)

add_definitions(${PNG_DEFINITIONS}) 

include_directories(${PNG_INCLUDE_DIR})

# Create a shared library
add_library(adonthell_gfx SHARED ${adonthell_gfx_SRCS})
target_link_libraries(adonthell_gfx
	adonthell_base
	${PNG_LIBRARY}
	)


################################
# Create the SDL Backend
set(gfx_sdl_SRCS
	sdl/gfx_sdl.cc
        sdl/screen_sdl.cc
        sdl/surface_sdl.cc
)

# Add specific include directory for this library.
include_directories(${SDL_INCLUDE_DIR})

add_library(sdl MODULE ${gfx_sdl_SRCS})
target_link_libraries(sdl
	${SDL_LIBRARY}
	adonthell_gfx
	)
set_target_properties(sdl PROPERTIES PREFIX "_")

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

add_library(sdlgl MODULE ${gfx_sdlgl_SRCS})
target_link_libraries(sdlgl
	${SDL_LIBRARY}
	${OPENGL_LIBRARY}
	adonthell_gfx
	)
set_target_properties(sdlgl PROPERTIES PREFIX "_")
ENDIF(OPENGL_FOUND)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_gfx)
adonthell_install_include(gfx "${adonthell_gfx_HEADERS}") 
adonthell_install_backend(gfx sdl)

IF(OPENGL_FOUND)
adonthell_install_backend(gfx sdlgl)
ENDIF(OPENGL_FOUND)
