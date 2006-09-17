# Define the adonthell_event_SRCS variable containing all required files.
SET(adonthell_event_SRCS
	date.cc
	event.cc
	factory.cc
	listener.cc
	listener_cxx.cc
	listener_python.cc
	types.cc
	time_event.cc
	time_event_manager.cc
)

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_event SHARED ${adonthell_event_SRCS})
