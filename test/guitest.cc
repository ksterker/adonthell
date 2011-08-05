#include <iostream>
#include <sstream>
#include <string>
using std::string;
#include "base/base.h"
#include "gfx/gfx.h"
#include "input/input.h"
#include "gui/gui.h"
#include "main/adonthell.h"
#include "gui/draw.h"

class InputHandler {
private:
public:
  
  bool letsexit ;
  gui::widget* widgetbase;
  
  InputHandler (gui::widget* wb = NULL) : letsexit (false), widgetbase(wb) {};
  
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
  void changelabel(bool down, void* arg) {
    gui::label* l = (gui::label*)arg;
	std::stringstream s; 
	static int count = 0;
	count++;
	s << "Click " << count;
	l->set_string(s.str());
  }
  void print(bool down, void* arg) {
    char* s = (char*) arg;
	std::cout << s << ":" << (down?"down":"up") << "\n";
  }
  int main () {
    
    // Initialize the gfx and input systems
    init_modules (GFX | INPUT);

    InputHandler ih;
        
    // Set us a nice window
	gfx::screen::set_fullscreen(false);
    gfx::screen::set_video_mode(512,512);
    std::cout << gfx::screen::info ();

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
	gui::font f;
	gui::font red;
	gui::label l(200, 30);
	l.set_string("This is a Label");
	gui::label label_with_bg("label.xml");
	label_with_bg.set_string("Is this readable?");

	gui::button b("button.xml");
	b.set_callback(base::make_functor(*this, &GuiTest::changelabel) , &l);
	b.set_string("Click Me");
	gui::button b2("button.xml");
	b2.set_callback(base::make_functor(*this, &GuiTest::print), (void*)"button 2");
	b2.set_string("Red Button");
	gui::button b3(300,30);
	b3.set_callback(base::make_functor(*this, &GuiTest::print), (void*)"button 3");
	b3.set_string("Button 3");
	b3.set_style("button.xml");
	gui::option o1("button.xml");
	o1.set_callback (base::make_functor(*this, &GuiTest::print), (void*)"option 1");
	o1.set_string("Option 1");

	gui::textbox t1(400, 30);
	
	gui::label multiline_test(400, 100);
	multiline_test.set_string("This is a test of the multiline label. In theory, once the text exceeds the width of the label,\nit should wrap to the next line.");
	multiline_test.set_multiline(true);
	multiline_test.set_center(true, true);

	gui::canvas cv(64, 64);
	cv.set_style("label.xml");
	cv.set_drawable(o1);

	/* arrange them in a freeform layout object */
	gui::layout widgets(0, 0);
	widgets.add_child( b, 10, 10);
	widgets.add_child(b2, 10, 50);
	widgets.add_child(b3, 10, 90);
	widgets.add_child(o1, 10,130);
	widgets.add_child(t1, 10,170);
    widgets.add_child(cv, 10,170);

	ih.widgetbase = &widgets;
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
	gfx::surface * screen = gfx::screen::get_surface();
	label_with_bg.set_color(screen->map_color(0xff,0,0,0xff));
	b2.set_color(screen->map_color(0xff,0,0,0xff));
    string ls = "I am typing a very long string that will not fit all the way within the alloted space";

    ::gui::window_manager::add(0, 0, &widgets);

    while (!ih.letsexit)
    {
        ::base::Timer.update ();
        ::input::manager::update();
      	::gui::window_manager::update();
	  	
		screen->fillrect(0, 0, screen->length(), screen->height(), 0);
		f.set_color(screen->map_color(0xff, 0,0,0xff));
		f.draw_text("Red Red Red", -10, 10);
		f.set_color(screen->map_color(0, 0xff,0,0xff));
		f.draw_text("Green Green", -10, 520);
		f.set_color(screen->map_color(0, 0,0xff,0xff));
		f.draw_text("Blue Blue B", 470, 520);
		f.set_color(screen->map_color(0xff, 0xff,0xff,0xff));
		f.draw_text("White White", 470, 10);
		/*
		std::vector<gui::textsize> ts;
		int w = 0, h = 0;
		f.getMultilineSize(ls, 200, ts, w, h);
		gui::box(10, 300, w, h, c, screen);
		int i;
		int p = 0;
		int y = 300;
		for (i = 0; i < ts.size(); i++)
		{
			y += ts[i].h;
			f.render(ls.substr(p, ts[i].cpos - p), 10, y, screen);
			p = ts[i].cpos +1;
		}
// */
      	l.draw(10, 50, NULL, screen);
		label_with_bg.draw(10, 80, NULL, screen);
		widgets.draw(10, 120, NULL, screen);
		multiline_test.draw(10, 350, NULL, screen);
		gui::box(10, 350, 400, 100, 0xff0000b0, screen);

		u_int32 c = 0xff0000b0;
		gui::box(345, 5, 30, 50, c, screen);
		gui::border(340, 5, 40, 55, screen);

		gfx::screen::update ();
    }
    
    // Do some cleanup, and we're ready to exit!

    return 0;
  }
} theApp;
