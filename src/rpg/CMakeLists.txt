# Define the adonthell_rpg_SRCS variable containing all required files.
set(adonthell_rpg_SRCS
	character.cc
	dialog.cc
	dialog_line.cc
	equipment.cc
	inventory.cc
	item_storage.cc
	item.cc
	log_entry.cc
	log_index.cc
	quest.cc
	quest_event.cc
	quest_event_manager.cc
	slot.cc
   race.cpp
   terrain_effects.cpp
)

set(adonthell_rpg_HEADERS
	character.h
	inventory.h
	log_index.h
	dialog.h
	item.h
	quest_event.h
	dialog_line.h
	item_storage.h
	quest_event_manager.h
	slot.h
	equipment.h
	log_entry.h
	quest.h
   race.h
   terrain_effects.h
)


# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_rpg SHARED ${adonthell_rpg_SRCS})
target_link_libraries(adonthell_rpg
	${PYTHON_LIBRARIES}
	adonthell_base
	adonthell_event
)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_rpg)
adonthell_install_include(rpg "${adonthell_rpg_HEADERS}") 
