#include <iostream>
#include "base/base.h"
#include "gfx/gfx.h"
#include "input/input.h"
#include "gui/gui.h"
#include "main/adonthell.h"


class Exit {
public:
  
  bool letsexit ;
  
  Exit () : letsexit (false) {};
  
  bool handle_keys (input::keyboard_event *ev) {
    // On escape, the global variable letsexit will be set to 1
    if (ev->key() == input::keyboard_event::ESCAPE_KEY) {
      letsexit = true;
      std::cout << "Escape pressed, leaving..." << std::endl;
      return true;
    }
    return false;
  }
};


class GuiTest : public adonthell::app {

  int main () {
    
    Exit myExit;
    
    // Initialize the gfx and input systems
    init_modules (GFX | INPUT);
    
    // Set us a nice window
    gfx::screen::set_video_mode(640, 480);
    
    // Create our input_listener and connect the callback
    // to handle keyboard events
    input::listener il;
    il.connect_keyboard_function(base::make_functor_ret(myExit, &Exit::handle_keys));
    input::manager::add (&il);
    
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    gui::container cont;
    cont.set_size (150, 150);
    cont.set_location (20, 30);
    
    for (unsigned int i = 0; i < 150; ++i) {
      gui::base * a = new gui::base ();
      a->set_size (40, 20);
      a->set_location (30, 10);
      cont.add_child (a);
    }
    
    gui::listlayout * layout = new gui::listlayout;
    cont.set_layout (layout);
    
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/

    // Run this loop until letsexit is set to 1 by the
    // callback function. Every time a key event is raised,
    // the input manager will send it to the listeners it handles
    // (that is, only the one we've connected) which will call the
    // callback function that has been connected to handle keyboard events.
    while (!myExit.letsexit) {
      ::base::Timer.update ();
      ::gfx::screen::update ();
      ::input::manager::update();
      cont.draw ();
    }
    
    // Do some cleanup, and we're ready to exit!
    cont.destroy_all ();

    return 0;
  }
} theApp;
