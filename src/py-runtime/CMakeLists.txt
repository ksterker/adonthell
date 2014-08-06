include(${SWIG_USE_FILE})

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# create swig runtime module
set_source_files_properties("py_runtime.i" PROPERTIES CPLUSPLUS ON)
set_source_files_properties("py_runtime.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_runtime")
swig_add_module(runtime python py_runtime.i)
swig_link_libraries(runtime ${PYTHON_LIBRARIES} adonthell_base)
set_target_properties (_runtime PROPERTIES COMPILE_FLAGS "${ADDITIONAL_SWIG_CXXFLAGS}")

# create dynamic library from swig runtime module
add_library(adonthell_py_runtime SHARED ${CMAKE_CURRENT_BINARY_DIR}/py_runtimePYTHON_wrap.cxx)
set_target_properties (adonthell_py_runtime PROPERTIES COMPILE_FLAGS "${ADDITIONAL_SWIG_CXXFLAGS}")
target_link_libraries(adonthell_py_runtime
	${PYTHON_LIBRARIES}
	)

# install libs
adonthell_install_lib (adonthell_py_runtime)

