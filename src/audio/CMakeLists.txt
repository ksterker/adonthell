# Define the adonthell_audio_SRCS variable containing all required files.
set(adonthell_audio_SRCS
	audio.cc
	sound.cc
	audio_event.cc
	audio_event_manager.cc
	audio_manager.cc
)

set(adonthell_audio_HEADERS
	audio_event.h
	audio_event_manager.h
	audio_manager.h
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
    ${LIBGLOG_LIBRARIES}
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

################################
# Unit tests
IF(DEVBUILD)
  add_executable(test_audio test_audio.cc)
  target_link_libraries(test_audio ${TEST_LIBRARIES} adonthell_audio)
  add_test(NAME Audio COMMAND test_audio)

  add_executable(test_audio_manager test_audio_manager.cc)
  target_link_libraries(test_audio_manager ${TEST_LIBRARIES} adonthell_audio)
  add_test(NAME AudioManger COMMAND test_audio_manager)

  add_executable(test_sound test_sound.cc)
  target_link_libraries(test_sound ${TEST_LIBRARIES} adonthell_audio)
  add_test(NAME AudioSound COMMAND test_sound)

  add_executable(test_audio_event test_audio_event.cc)
  target_link_libraries(test_audio_event ${TEST_LIBRARIES} adonthell_audio)
  add_test(NAME AudioEvent COMMAND test_audio_event)
ENDIF(DEVBUILD)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_audio)
adonthell_install_include(audio "${adonthell_audio_HEADERS}")
adonthell_install_backend(audio audio-backend-sdl)
