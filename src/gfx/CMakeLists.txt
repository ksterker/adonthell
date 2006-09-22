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

add_library(sdl SHARED ${gfx_sdl_SRCS})
target_link_libraries(sdl
	${SDL_LIBRARY}
	adonthell_gfx
	)



#############################################
# Install Stuff
adonthell_install_lib(adonthell_gfx)
adonthell_install_include(gfx "${adonthell_gfx_HEADERS}") 
adonthell_install_backend(gfx sdl)
