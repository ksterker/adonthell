#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <sstream>
#include "base/base.h"
#include "gfx/gfx.h"
#include "input/input.h"
#include "gui/gui.h"
#include "main/adonthell.h"
#include "gui/draw.h"
#include "rpg/dialog.h"

class InputHandler {
private:
public:
  
  bool letsexit ;
  gui::base* widgetbase;
  
  InputHandler (gui::widget* wb = NULL) : letsexit (false), widgetbase(wb) {};
  
  bool handle_keys (input::keyboard_event *ev) {
    // On escape, the global variable letsexit will be set to 1
    if (ev->key() == input::keyboard_event::ESCAPE_KEY) {
      letsexit = true;
      std::cout << "Escape pressed, leaving..." << std::endl;
      return true;
    } else if (ev->type() == input::keyboard_event::KEY_PUSHED) {
		if (widgetbase) widgetbase->keydown(*ev);
	} else if (ev->type() == input::keyboard_event::KEY_RELEASED) {
		if (widgetbase) widgetbase->keyup(*ev);
	}
    return false;
  }
};


class DialogTest : public adonthell::app {
  InputHandler ih;
    
  static void changelabel(bool down, void* arg) {
    gui::label* l = (gui::label*)arg;
	std::stringstream s; 
	static int count = 0;
	count++;
	s << "Click " << count;
	l->setString(s.str());
  }
  static void print(bool down, void* arg) {
    char* s = (char*) arg;
	std::cout << s << ":" << (down?"down":"up") << "\n";
  }
  void shutdown() {
	ih.letsexit = true;
  }
  int main () {
    // Initialize the gfx and input systems
    init_modules (GFX | INPUT | PYTHON);
    // Set us a nice window
	gfx::screen::set_fullscreen(false);
    gfx::screen::set_video_mode(512,512);
	python::add_search_path(base::Paths.user_data_dir() + "data/");
  	python::import_module("adonthell.rpg");
	/* try to load a dialog */
   // load rpg side
	rpg::character npc("NPC", "Id", rpg::NPC, "Humans");
	npc.set_dialogue("tech_preview");
	rpg::character player("Player", "Player", rpg::PLAYER, "Humans");
	//rpg::dialog dlg(npc);
    
    // Create our input_listener and connect the callback
    // to handle keyboard events
    input::listener il;
    il.connect_keyboard_function(base::make_functor_ret(ih, &InputHandler::handle_keys));
    input::manager::add (&il);
    
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
	gui::conversation conv(npc, 500, 300, base::make_functor(*this, &DialogTest::shutdown));
	conv.focus();	
	ih.widgetbase = &conv;
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/
    /*************************************************************************/

    gfx::surface *screen = gfx::screen::get_surface();

    // Run this loop until letsexit is set to 1 by the
    // callback function. Every time a key event is raised,
    // the input manager will send it to the listeners it handles
    // (that is, only the one we've connected) which will call the
    // callback function that has been connected to handle keyboard events.

    while (!ih.letsexit) {
      ::base::Timer.update ();
      ::input::manager::update();
	  	
		screen->fillrect(0, 0, screen->length(), screen->height(), 0);
      	conv.draw(10, 10, screen);

		gfx::screen::update ();
    }
    
    // Do some cleanup, and we're ready to exit!
    rpg::specie::cleanup();
    return 0;
  }
} theApp;
