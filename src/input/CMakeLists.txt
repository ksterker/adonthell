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
set(input_sdl_SRCS
     sdl/input_sdl.cc
     sdl/manager_sdl.cc
)

# Add specific include directory for this library.
include_directories(${SDL_INCLUDE_DIR})


add_library(sdl MODULE ${input_sdl_SRCS})
target_link_libraries(sdl
	${SDL_LIBRARY}
	adonthell_input
	)
set_target_properties(sdl PROPERTIES PREFIX "" SUFFIX ".so")


#############################################
# Install Stuff
adonthell_install_lib(adonthell_input)
adonthell_install_include(input "${adonthell_input_HEADERS}") 
adonthell_install_backend(input sdl)
