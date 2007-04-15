include(${SWIG_USE_FILE})

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# create swig runtime module
set_source_files_properties("py_runtime.i" PROPERTIES CPLUSPLUS ON)
set_source_files_properties("py_runtime.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_runtime")
swig_add_module(runtime python py_runtime.i)
swig_link_libraries(runtime ${PYTHON_LIBRARIES})

# create dynamic library from swig runtime module
add_library(adonthell_py_runtime SHARED ${CMAKE_CURRENT_BINARY_DIR}/py_runtimePYTHON_wrap.cxx)
target_link_libraries(adonthell_py_runtime
	${PYTHON_LIBRARIES}
	)

# create debug module
set_source_files_properties("py_debug.i" PROPERTIES CPLUSPLUS ON)
set_source_files_properties("py_debug.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;debug")
swig_add_module(debug python py_debug.i)
swig_link_libraries(debug ${PYTHON_LIBRARIES} adonthell_py_runtime)
set_target_properties (_debug PROPERTIES INSTALL_RPATH "${LIB_INSTALL_RPATH}")

# install modules
adonthell_install_py (_debug "debug.py")
adonthell_install_lib (adonthell_py_runtime)