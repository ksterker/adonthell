# Adonthell Main Stuff

set(adonthell_main_SRCS
	adonthell.cc
        main.cc
)

set(adonthell_main_HEADERS
	adonthell.h
)

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH} ${LIBGLOG_INCLUDE_PATH})

add_definitions(-DPYTHONSPDIR="${ESCAPE}${PYTHON_SITE_PACKAGE_DIR}${ESCAPE}")

# Create a shared library
add_library(adonthell_main SHARED ${adonthell_main_SRCS})
target_link_libraries(adonthell_main
	${PYTHON_LIBRARIES}
    ${LIBGLOG_LIBRARIES}
	adonthell_base
	adonthell_gfx
	adonthell_gui
	adonthell_audio
	adonthell_input
	adonthell_python
	adonthell_world
	)


################################
# Create the SDL Backend

## In function of the os, use a specific sdl wrapper.
if(APPLE)
	set(sdl_SRCS
		sdl/osx.m)
    set_source_files_properties (sdl/osx.m PROPERTIES COMPILE_FLAGS -ObjC++)
else(APPLE)
	if(WIN32)
		set(sdl_SRCS
			sdl/winnt.cc)
	else(WIN32)
		set(sdl_SRCS
			sdl/linux.cc)
	endif(WIN32)
endif(APPLE)

include_directories(${SDL_INCLUDE_DIR})

add_library(main-backend-sdl MODULE ${sdl_SRCS})
set_target_properties(main-backend-sdl PROPERTIES PREFIX "_" OUTPUT_NAME "sdl")
if(APPLE)
set_target_properties(main-backend-sdl PROPERTIES LINK_FLAGS "-framework Cocoa")
endif(APPLE)
target_link_libraries(main-backend-sdl
	${SDL_LIBRARY}
	adonthell_main
)


#############################################
# Install Stuff
adonthell_install_lib(adonthell_main)
adonthell_install_include(main "${adonthell_main_HEADERS}")
adonthell_install_backend(main main-backend-sdl)
