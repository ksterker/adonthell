# Define the adonthell_gui_SRCS variable containing all required files.
SET(adonthell_gui_SRCS
	base.cc
	container.cc
	layout.cc
	listlayout.cc
)

# Create a shared library
add_library(adonthell_gui SHARED ${adonthell_gui_SRCS})
target_link_libraries(adonthell_gui
	adonthell_base
	adonthell_gfx
	adonthell_input	
	)
