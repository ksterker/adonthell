include(${SWIG_USE_FILE})

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

set(PY_MODULES
#   runtime
   base
   debug
   event
   gfx
   gui
   input
   rpg
)


   

# Runtime library
# This library should be created with add_library - still, SWIG should be invoked to create the .cc file.
set_source_files_properties("py_runtime.i" PROPERTIES CPLUSPLUS ON)
set_source_files_properties("py_runtime.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_runtime")
swig_add_module(runtime python "py_runtime.i")
#add_library(adonthell_py_runtime SHARED "py_runtimePYTHON_wrap.cxx")
#target_link_libraries(adonthell_py_runtime
#	${PYTHON_LIBRARIES}
#	)

set_target_properties(${SWIG_MODULE_runtime_REAL_NAME} PROPERTIES PREFIX "libadonthell_py")
swig_link_libraries(runtime ${PYTHON_LIBRARIES})
adonthell_install_lib(${SWIG_MODULE_runtime_REAL_NAME})

# Wrappers modules
foreach(name base event gfx input audio gui)
  set_source_files_properties("py_${name}.i" PROPERTIES CPLUSPLUS ON)
  set_source_files_properties("py_${name}.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_${name}")
  swig_add_module(${name} python "py_${name}.i")
  # Need to fix and improve swig macro ...
  #  swig_link_libraries(${name} ${PYTHON_LIBRARIES} "_runtime.so")
endforeach(name)
