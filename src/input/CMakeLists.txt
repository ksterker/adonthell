# Define the adonthell_input_SRCS variable containing all required files.
set(adonthell_input_SRCS
	input.cc
	event.cc
	joystick_event.cc
	mouse_event.cc
	keyboard_event.cc
	control_event.cc
	listener.cc
	manager.cc
)


set(adonthell_input_HEADERS
	control_event.h
	input.h
	keyboard_event.h
	manager.h
	event.h
	joystick_event.h
	listener.h
	mouse_event.h
)


# Create a shared library
add_library(adonthell_input SHARED ${adonthell_input_SRCS})
target_link_libraries(adonthell_input
	adonthell_base
	)


################################
# Create the SDL Backend
IF(${SDL_VERSION} STREQUAL 2.0)
set(input_sdl_SRCS
     sdl2/input_sdl.cc
     sdl2/manager_sdl.cc
)
ELSE(${SDL_VERSION} STREQUAL 2.0)
set(input_sdl_SRCS
     sdl/input_sdl.cc
     sdl/manager_sdl.cc
)
ENDIF(${SDL_VERSION} STREQUAL 2.0)

# Add specific include directory for this library.
include_directories(${SDL_INCLUDE_DIR})


add_library(input-backend-sdl MODULE ${input_sdl_SRCS})
target_link_libraries(input-backend-sdl
	${SDL_LIBRARY}
	adonthell_input
	)
set_target_properties(input-backend-sdl PROPERTIES PREFIX "_" OUTPUT_NAME "sdl")


#############################################
# Install Stuff
adonthell_install_lib(adonthell_input)
adonthell_install_include(input "${adonthell_input_HEADERS}")
adonthell_install_backend(input input-backend-sdl)
