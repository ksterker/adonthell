# Define the adonthell_base_SRCS variable containing all required files.
SET(adonthell_base_SRCS
	base.cc
        callback.cc
	configuration.cc
	configio.cc
	diskio.cc
	file.cc
        flat.cc
	nls.cc
        paths.cc
        timer.cc
)

# Add specific include directory for this library.
include_directories(${LIBXML2_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_base SHARED ${adonthell_base_SRCS})
