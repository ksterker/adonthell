# Define the adonthell_gui_SRCS variable containing all required files.
set(adonthell_gui_SRCS
	base.cc
	container.cc
	layout.cc
	listlayout.cc
)

set(adonthell_gui_HEADERS
	base.h
	container.h
	gui.h
	layout.h
	listlayout.h
)


# Create a shared library
add_library(adonthell_gui SHARED ${adonthell_gui_SRCS})
target_link_libraries(adonthell_gui
	adonthell_base
	adonthell_gfx
	adonthell_input	
	)


#############################################
# Install Stuff
adonthell_install_lib(adonthell_gui)
adonthell_install_include(gui "${adonthell_gui_HEADERS}") 
