# Define the adonthell_gui_SRCS variable containing all required files.
set(adonthell_gui_SRCS
	button.cpp
	font.cpp
	label.cpp
	layout.cc
	option.cpp
	textbox.cpp
	widget.cpp
)

set(adonthell_gui_HEADERS
    base.h
	button.h
	draw.h
	font.h
	gui.h
	label.h
	layout.h
	option.h
	widget.h
)

# Create a shared library
add_library(adonthell_gui SHARED ${adonthell_gui_SRCS})
set_target_properties(adonthell_gui PROPERTIES COMPILE_FLAGS ${LIBFT_CFLAGS})
target_link_libraries(adonthell_gui
    ${LIBFT_LIBRARIES}
	adonthell_base
	adonthell_gfx
	adonthell_input	
	)


#############################################
# Install Stuff
adonthell_install_lib(adonthell_gui)
adonthell_install_include(gui "${adonthell_gui_HEADERS}") 
