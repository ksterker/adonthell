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
# Try to build the guitest
ADD_EXECUTABLE(guitest
			guitest.cc)

set_target_properties(guitest PROPERTIES COMPILE_FLAGS ${LIBFT_CFLAGS})

TARGET_LINK_LIBRARIES(guitest
	ltdl
	adonthell_base
	adonthell_gfx
	adonthell_gui
	adonthell_input
	adonthell_main
#	adonthell_python
#	adonthell_py_runtime
	${PYTHON_EXTRA_LIBRARIES}
	${LIBFT_LIBRARIES}
	)

###############################
# Try to build the dialogtest
ADD_EXECUTABLE(dialogtest
			dialogtest.cc)

set_target_properties(dialogtest PROPERTIES COMPILE_FLAGS ${LIBFT_CFLAGS})

TARGET_LINK_LIBRARIES(dialogtest
	ltdl
	adonthell_base
	adonthell_gfx
	adonthell_gui
	adonthell_input
	adonthell_main
	adonthell_python
	adonthell_py_runtime
	${PYTHON_EXTRA_LIBRARIES}
	${PYTHON_EXTRA_LIBRARIES}
	)

###############################
# Try to build the worldtest
ADD_EXECUTABLE(worldtest
			worldtest.cc)

include_directories(${PYTHON_INCLUDE_PATH})

TARGET_LINK_LIBRARIES(worldtest
	ltdl
	adonthell_base
	adonthell_gfx
	adonthell_input
	adonthell_main
	adonthell_world

#	adonthell_python
#	adonthell_py_runtime
	${PYTHON_EXTRA_LIBRARIES}
	)

###############################
# Try to build the path_test
ADD_EXECUTABLE(path_test
			path_test.cc)

include_directories(${PYTHON_INCLUDE_PATH})

TARGET_LINK_LIBRARIES(path_test
	ltdl
	adonthell_base
	adonthell_gfx
	adonthell_input
	adonthell_main
	adonthell_world

#	adonthell_python
#	adonthell_py_runtime
	${PYTHON_EXTRA_LIBRARIES}
	)


###############################
# Try to build the smallworld
ADD_EXECUTABLE(smallworld
			smallworld.cc)

include_directories(${PYTHON_INCLUDE_PATH})

TARGET_LINK_LIBRARIES(smallworld
	ltdl
	adonthell_base
	adonthell_gfx
	adonthell_input
	adonthell_main
	adonthell_world

#	adonthell_python
#	adonthell_py_runtime
	${PYTHON_EXTRA_LIBRARIES}
	)
