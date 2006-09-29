# Add the current directory as include source.
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
)


# Define the Adonthell libraries
set(ADONTHELL_MODULES
	base
	event
	gfx
	input
	audio
	gui
	rpg
	main
)


#############################
# Define some macro helpers
macro(adonthell_install_lib libname)
   install(TARGETS ${libname} DESTINATION "lib")
endmacro(adonthell_install_lib)

macro(adonthell_install_include name headers)
   install(FILES ${headers} DESTINATION "include/adonthell/${name}")
endmacro(adonthell_install_include)

macro(adonthell_install_backend name lib)
   install(TARGETS ${lib} DESTINATION "lib/adonthell/${name}")
endmacro(adonthell_install_backend)


macro(adonthell_install_py libs files)
   install(TARGETS ${libs} DESTINATION "lib/${PYTHON_VERSION_DIR}/site-packages/adonthell")
endmacro(adonthell_install_py)


add_subdirectory(python)

foreach(lib ${ADONTHELL_MODULES})
  add_subdirectory(${lib})
endforeach(lib)

add_subdirectory(py-wrappers)
