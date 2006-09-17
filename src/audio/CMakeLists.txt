# Define the adonthell_audio_SRCS variable containing all required files.
SET(adonthell_audio_SRCS
	audio.cc
	sound.cc
	audio_event.cc
	audio_event_manager.cc
)

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_audio SHARED ${adonthell_audio_SRCS})
