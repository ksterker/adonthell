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

target_link_libraries(adonthell_audio
	${PYTHON_LIBRARIES}
	adonthell_base
	adonthell_event
	)

################################
# Create the SDL Backend
set(audio_sdl_SRCS
	sdl/audio_sdl.cc
)

# Add specific include directory for this library.
include_directories(${SDL_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR})


add_library(sdl SHARED ${audio_sdl_SRCS})
target_link_libraries(sdl
	${SDL_LIBRARY}
	${SDLMIXER_LIBRARY}
	adonthell_audio
	)
