#include <iostream>
#include "gfx/gfx.h"
#include "input/input.h"
#include "main/adonthell.h"

class myClass
{
public:
    // Our exit variable
    bool letsexit;

    myClass () : letsexit(false) { };
    
    // Callback fonction to handle keyboard events.
    // It will be passed a keyboard_event as a parameter
    // and is supposed to return nonzero if it took the
    // event.
    bool handle_keys(input::keyboard_event * ev)
    {
        // If the key is released
        if (ev->type() == input::keyboard_event::KEY_RELEASED)
            std::cout << "Key " << ev->key_symbol() << " has been released." << std::endl;
        
        // If the key is pushed
        else if (ev->type() == input::keyboard_event::KEY_PUSHED)
        {
            // Print some info about what happened
            std::cout << "Key " << ev->key_symbol() << " has been pushed, unichar emitted is ";
            if (!ev->unikey()) std::cout << "<none>" << std::endl;
            else std::cout << (unsigned char) ev->unikey() << " (" << (unsigned int) ev->unikey() << ")" << std::endl;

            // On escape, the global variable letsexit will be set to 1
            if (ev->key() == input::keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
                std::cout << "Escape pressed, leaving..." << std::endl;
            }
        }
        return true;
    }
};
        
class AdonthellApp : public adonthell::app
{
    int main () const
    {
        myClass myObject;
        
        // Initialize the gfx and input systems
        gfx::init("sdl");
        input::init("sdl");
    
        // Set us a nice window
        gfx::screen::set_video_mode(640, 480);
        
        // Create our input_listener and connect the callback
        // to handle keyboard events
        input::listener il;
        il.connect_keyboard_function(base::make_functor_ret(myObject, &myClass::handle_keys));
        
        // Add the listener to the manager
        input::manager::add(&il);
        
        // Run this loop until letsexit is set to 1 by the
        // callback function. Every time a key event is raised,
        // the input manager will send it to the listeners it handles
        // (that is, only the one we've connected) which will call the
        // callback function that has been connected to handle keyboard events.
        while (!myObject.letsexit)
            input::manager::update();
    
        // Do some cleanup, and we're ready to exit!
        input::cleanup();
        gfx::cleanup();
        
        return 0;
    }

} theApp;
