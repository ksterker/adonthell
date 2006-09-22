include(${SWIG_USE_FILE})

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

set(PY_MODULES
   runtime
   base
   debug
   event
   gfx
   gui
   input
   rpg
)


   

# Runtime
set_source_files_properties("py_runtime.i" PROPERTIES CPLUSPLUS ON)
set_source_files_properties("py_runtime.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_runtime")
swig_add_module(runtime python "py_runtime.i")
swig_link_libraries(runtime ${PYTHON_LIBRARIES})



foreach(name base event gfx input audio gui)
  set_source_files_properties("py_${name}.i" PROPERTIES CPLUSPLUS ON)
  set_source_files_properties("py_${name}.i" PROPERTIES SWIG_FLAGS "-noexcept;-nocontract;-modern;-module;py_${name}")
  swig_add_module(${name} python "py_${name}.i")
  # Need to fix and improve swig macro ...
  #  swig_link_libraries(${name} ${PYTHON_LIBRARIES} "_runtime.so")
endforeach(name)
