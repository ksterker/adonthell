# Define the adonthell_input_SRCS variable containing all required files.
SET(adonthell_input_SRCS
	input.cc
	event.cc
	joystick_event.cc
	mouse_event.cc
	keyboard_event.cc
	control_event.cc
	listener.cc
	manager.cc
)

# Create a shared library
add_library(adonthell_input SHARED ${adonthell_input_SRCS})
target_link_libraries(adonthell_input
	adonthell_base
	)


################################
# Create the SDL Backend
set(input_sdl_SRCS
     sdl/input_sdl.cc
     sdl/manager_sdl.cc
)

# Add specific include directory for this library.
include_directories(${SDL_INCLUDE_DIR})


add_library(sdl SHARED ${input_sdl_SRCS})
target_link_libraries(sdl
	${SDL_LIBRARY}
	adonthell_input
	)
