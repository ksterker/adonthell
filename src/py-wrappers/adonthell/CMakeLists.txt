include(${SWIG_USE_FILE})

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Wrapper modules
foreach(name audio base event gfx gui input main rpg)
  set_source_files_properties("py_${name}.i" PROPERTIES CPLUSPLUS ON)
  set_source_files_properties("py_${name}.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_${name}")
  
  if(${name} STREQUAL main)
    swig_add_module(${name} python "py_${name}.i" "py_main.h" "py_main.cc")
  else(${name} STREQUAL main)
    swig_add_module(${name} python "py_${name}.i")
  endif(${name} STREQUAL main)
  
  swig_link_libraries(${name} ${PYTHON_LIBRARIES} adonthell_${name})
endforeach(name)

# input wrapper needs additional libraries
swig_link_libraries(input adonthell_python adonthell_py_runtime)

# main module needs py_main.h to compile in out-of-source build
set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/py_mainPYTHON_wrap.cxx" PROPERTIES COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}")
