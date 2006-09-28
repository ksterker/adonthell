###############################
# Add include directory
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

###############################
# Try to build the CallbackTest
ADD_EXECUTABLE(callbacktest 
			callbacktest.cc)

TARGET_LINK_LIBRARIES(callbacktest
	ltdl
	adonthell_base
	)

###############################
# Try to build the diskiotest
ADD_EXECUTABLE(diskiotest
			diskiotest.cc)

TARGET_LINK_LIBRARIES(diskiotest
	ltdl
	adonthell_base
	)

###############################
# Try to build the gfxtest
ADD_EXECUTABLE(gfxtest
			gfxtest.cc)

TARGET_LINK_LIBRARIES(gfxtest
	ltdl
	adonthell_base
	adonthell_gfx
	)
	