include(${SWIG_USE_FILE})

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

SET_SOURCE_FILES_PROPERTIES(py_runtime.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(py_runtime.i PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_runtime")
SWIG_ADD_MODULE(runtime python py_runtime.i)
