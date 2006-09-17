# Define the adonthell_gfx_SRCS variable containing all required files.
SET(adonthell_gfx_SRCS
	gfx.cc
	drawing_area.cc
	drawable.cc
	png_wrapper.cc
	surface.cc
	screen.cc
	animation.cc
)

# Create a shared library
add_library(adonthell_gfx SHARED ${adonthell_gfx_SRCS})
