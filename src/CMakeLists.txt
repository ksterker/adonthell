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
    world
)

#
# Define some macro helpers
#
macro(adonthell_install_lib libname)
    install(TARGETS ${libname} 
    	RUNTIME DESTINATION "bin"
   		LIBRARY DESTINATION "lib"
   		ARCHIVE DESTINATION "lib")
endmacro(adonthell_install_lib)

macro(adonthell_install_include name headers)
   install(FILES ${headers} DESTINATION "include/adonthell-${VERSION}/adonthell/${name}")
endmacro(adonthell_install_include)

macro(adonthell_install_backend name lib)
   install(TARGETS ${lib} DESTINATION "lib/adonthell/${name}")
endmacro(adonthell_install_backend)

macro(adonthell_install_py libs scripts)
   install(TARGETS ${libs} DESTINATION "${PYTHON_SITE_PACKAGE_DIR}/adonthell")
   install(FILES ${scripts} DESTINATION "${PYTHON_SITE_PACKAGE_DIR}/adonthell")
endmacro(adonthell_install_py)

#
# Build subdirectories
#
add_subdirectory(python)

foreach(lib ${ADONTHELL_MODULES})
  add_subdirectory(${lib})
endforeach(lib)

add_subdirectory(py-runtime)
add_subdirectory(py-wrappers)
