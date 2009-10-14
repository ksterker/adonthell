include(${SWIG_USE_FILE})

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# create debug module
set_source_files_properties("py_debug.i" PROPERTIES CPLUSPLUS ON)
set_source_files_properties("py_debug.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;debug")
swig_add_module(debug python py_debug.i)
swig_link_libraries(debug ${PYTHON_LIBRARIES} adonthell_py_runtime)
set_target_properties (_debug PROPERTIES INSTALL_RPATH "${LIB_INSTALL_RPATH}")
adonthell_install_py (_debug "${CMAKE_CURRENT_BINARY_DIR}/debug.py")


# Wrapper modules
foreach(name audio base event gfx gui input main rpg world)
  set_source_files_properties("py_${name}.i" PROPERTIES CPLUSPLUS ON)
  set_source_files_properties("py_${name}.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;${name}")

  if(${name} STREQUAL main)
    swig_add_module(${name} python "py_${name}.i" "py_main.h" "py_main.cc")
  else(${name} STREQUAL main)
    FILE (GLOB SWIG_MODULE_${name}_EXTRA_DEPS ${CMAKE_SOURCE_DIR}/src/${name}/*.h)
    swig_add_module(${name} python "py_${name}.i")
  endif(${name} STREQUAL main)

  if(${name} STREQUAL base)
    swig_link_libraries(base ${PYTHON_LIBRARIES} adonthell_python)
  endif(${name} STREQUAL base)

  swig_link_libraries(${name} ${PYTHON_LIBRARIES} adonthell_${name})
  set_target_properties (_${name} PROPERTIES INSTALL_RPATH "${LIB_INSTALL_RPATH}")
  adonthell_install_py (_${name} "${CMAKE_CURRENT_BINARY_DIR}/${name}.py")
endforeach(name)

# input wrapper needs additional libraries
swig_link_libraries(input adonthell_python adonthell_py_runtime)

# main module needs py_main.h to compile in out-of-source build
set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/py_mainPYTHON_wrap.cxx" PROPERTIES COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}")

# gui module requires freetype2 includes
set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/py_guiPYTHON_wrap.cxx" PROPERTIES COMPILE_FLAGS "${LIBFT_CFLAGS}")

# need __init__.py for modules being recognized correctly
write_file (${CMAKE_CURRENT_BINARY_DIR}/__init__.py "\n")
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py DESTINATION ${PYTHON_SITE_PACKAGE_DIR}/adonthell)
