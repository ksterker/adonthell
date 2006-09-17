# Add the current directory as include source.
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
)


# Define the Adonthell libraries
set(ADONTHELL_LIBS
	main
	base
	event
	gfx
	input
	audio
	gui
	rpg
)

add_subdirectory(python)

foreach(lib ${ADONTHELL_LIBS})
  add_subdirectory(${lib})
endforeach(lib)

add_subdirectory(py-wrappers)
