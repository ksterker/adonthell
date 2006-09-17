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
