# Define the adonthell_python_SRCS variable containing all required files.
SET(adonthell_python_SRCS
	callback.cc
    method.cc
    pool.cc
    python.cc
    script.cc
)

SET(adonthell_python_HEADERS
	callback.h
	callback_support.h
    method.h
    pool.h
    python.h
    script.h
)


# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_python SHARED ${adonthell_python_SRCS})
target_link_libraries(adonthell_python
    ${PYTHON_LIBRARIES}
    adonthell_base)

################################
# Unit tests
IF(DEVBUILD)
  add_executable(test_python test_python.cc)
  target_link_libraries(test_python ${TEST_LIBRARIES} adonthell_python)
  add_test(NAME Python COMMAND test_python)
ENDIF(DEVBUILD)

################################  
# install
adonthell_install_lib (adonthell_python)
adonthell_install_include(python "${adonthell_python_HEADERS}") 