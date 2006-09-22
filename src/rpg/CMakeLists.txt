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
	schedule.cc
	schedule_data.cc
	slot.cc
)

set(adonthell_rpg_HEADERS
	character.h
	inventory.h
	log_index.h
	schedule_data.h
	dialog.h
	item.h
	quest_event.h
	schedule.h
	dialog_line.h
	item_storage.h
	quest_event_manager.h
	slot.h
	equipment.h
	log_entry.h
	quest.h
)


# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_rpg SHARED ${adonthell_rpg_SRCS})
target_link_libraries(adonthell_rpg
	${PYTHON_LIBRARIES}
	adonthell_base
)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_rpg)
adonthell_install_include(rpg "${adonthell_rpg_HEADERS}") 
