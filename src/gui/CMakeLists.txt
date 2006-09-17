# Define the adonthell_gui_SRCS variable containing all required files.
SET(adonthell_gui_SRCS
	base.cc
	container.cc
	layout.cc
	listlayout.cc
)

# Create a shared library
add_library(adonthell_gui SHARED ${adonthell_gui_SRCS})
