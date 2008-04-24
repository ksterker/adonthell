# Define the adonthell_audio_SRCS variable containing all required files.
set(adonthell_audio_SRCS
	audio.cc
	sound.cc
	audio_event.cc
	audio_event_manager.cc
)

set(adonthell_audio_HEADERS
	audio_event.h
	audio_event_manager.h
	audio.h
	sound.h
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


add_library(audio-backend-sdl MODULE ${audio_sdl_SRCS})
target_link_libraries(audio-backend-sdl
	${SDL_LIBRARY}
	${SDLMIXER_LIBRARY}
	adonthell_audio
	)
set_target_properties(audio-backend-sdl PROPERTIES PREFIX "_" OUTPUT_NAME "sdl")


#############################################
# Install Stuff
adonthell_install_lib(adonthell_audio)
adonthell_install_include(audio "${adonthell_audio_HEADERS}")
adonthell_install_backend(audio audio-backend-sdl)