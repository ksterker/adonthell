# Define the adonthell_rpg_SRCS variable containing all required files.
SET(adonthell_rpg_SRCS
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

# Add specific include directory for this library.
include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_rpg SHARED ${adonthell_rpg_SRCS})
