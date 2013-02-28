# Define the adonthell_rpg_SRCS variable containing all required files.
set(adonthell_rpg_SRCS
	character.cc
	dialog.cc
	dialog_line.cc
	equipment.cc
	faction.cc
	inventory.cc
	item_storage.cc
	item.cc
	log_entry.cc
	log_index.cc
	quest.cc
	quest_event.cc
	quest_event_manager.cc
	pathfinding_costs.cc
	slot.cc
)

set(adonthell_rpg_HEADERS
	character.h
	dialog.h
	dialog_line.h
	equipment.h
	faction.h
	inventory.h
	item.h
	item_storage.h
	log_entry.h
	log_index.h
	quest.h
	quest_event.h
	quest_event_manager.h
	pathfinding_costs.h
	slot.h
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
