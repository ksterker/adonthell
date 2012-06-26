# Define the adonthell_gui_SRCS variable containing all required files.
set(adonthell_gui_SRCS
	button.cpp
	canvas.cc
	conversation.cc
    decoration.cc
	font.cpp
	fontcache.cc
	indicatorbar.cc
	label.cpp
	layout.cc
    listlayout.cc
	option.cpp
	scrollview.cc
	textbox.cpp
	ui_event.cc
	ui_event_manager.cc
	widget.cpp
    window_manager.cc
)

set(adonthell_gui_HEADERS
	button.h
	canvas.h
	conversation.h
    decoration.h
	font.h
    fontcache.h
	gui.h
    indicatorbar.h
	label.h
	layout.h
    listlayout.h
	option.h
    scrollview.h
    textbox.h
    ui_event.h
    ui_event_manager.h
	widget.h
    window_manager.h
)

include_directories(${PYTHON_INCLUDE_PATH})

# Create a shared library
add_library(adonthell_gui SHARED ${adonthell_gui_SRCS})
set_target_properties(adonthell_gui PROPERTIES COMPILE_FLAGS ${LIBFT_CFLAGS})
target_link_libraries(adonthell_gui
    ${LIBFT_LIBRARIES}
    ${PYTHON_LIBRARIES}
	adonthell_base
    adonthell_event
	adonthell_gfx
	adonthell_input	
	adonthell_rpg	
	)

################################
# Unit tests
IF(DEVBUILD)
  add_executable(test_ui_event_manager test_ui_event_manager.cc)
  set_target_properties(test_ui_event_manager PROPERTIES COMPILE_FLAGS ${LIBFT_CFLAGS})
  target_link_libraries(test_ui_event_manager ${TEST_LIBRARIES} ${LIBFT_LIBRARIES} adonthell_gui)
  add_test(NAME UiEventManager COMMAND test_ui_event_manager)

  add_executable(test_ui_event test_ui_event.cc)
  set_target_properties(test_ui_event PROPERTIES COMPILE_FLAGS ${LIBFT_CFLAGS})
  target_link_libraries(test_ui_event ${TEST_LIBRARIES} adonthell_gui)
  add_test(NAME UiEvent COMMAND test_ui_event)
ENDIF(DEVBUILD)

#############################################
# Install Stuff
adonthell_install_lib(adonthell_gui)
adonthell_install_include(gui "${adonthell_gui_HEADERS}") 
