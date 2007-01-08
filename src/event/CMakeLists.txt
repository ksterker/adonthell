# Define the adonthell_event_SRCS variable containing all required files.
set(adonthell_event_SRCS
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

set(adonthell_event_HEADERS
	date.h
	listener_cxx.h
	manager_base.h
	time_event_manager.h
	event.h
	listener.h
	manager.h
	types.h
	factory.h
	listener_python.h
	time_event.h
)

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_event SHARED ${adonthell_event_SRCS})
target_link_libraries(adonthell_event
	${PYTHON_LIBRARIES}
	adonthell_base
	adonthell_python
	adonthell_py_runtime
	)


#############################################
# Install Stuff
adonthell_install_lib(adonthell_event)
adonthell_install_include(event "${adonthell_event_HEADERS}") 
