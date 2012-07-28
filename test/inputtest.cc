#include <iostream>
#include <adonthell/gfx/gfx.h>
#include <adonthell/base/base.h>
#include <adonthell/input/input.h>
#include <adonthell/main/adonthell.h>


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
            std::cout << "Key " << ev->key_symbol() << " has been pushed." << std::endl;
            // On escape, the global variable letsexit will be set to 1
            if (ev->key() == input::keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
                std::cout << "Escape pressed, leaving..." << std::endl;
            }
        }
        
        else
        {
            std::cout << "Unichar emitted is " << ev->unikey() << "." << std::endl;
        }
        
        return true;
    }

    bool handle_joystick(input::joystick_event * ev)
    {
        // Print some info about what happened
        if (ev->type() == input::joystick_event::BUTTON_RELEASED)
        {
            std::cout << "Button " << ev->button_symbol() << " has been released." << std::endl;
        }
        else if (ev->type() == input::joystick_event::BUTTON_PUSHED)
        {
            std::cout << "Button " << ev->button_symbol() << " has been pushed." << std::endl;
        }
        return true;
    }
};
        
class AdonthellApp : public adonthell::app
{
    int main ()
    {
        myClass myObject;
        
        // Initialize the gfx and input systems
        init_modules (GFX | INPUT);
    
        // Set us a nice window
        gfx::screen::set_video_mode(640, 480);
        
        // Create our input_listener and connect the callback
        // to handle keyboard events
        input::listener il;
        il.connect_keyboard_function(base::make_functor_ret(myObject, &myClass::handle_keys));
        il.connect_joystick_function(base::make_functor_ret(myObject, &myClass::handle_joystick));
        
        // Add the listener to the manager
        input::manager::add(&il);
        
        input::manager::enable_unicode(true);
        
        // Run this loop until letsexit is set to 1 by the
        // callback function. Every time a key event is raised,
        // the input manager will send it to the listeners it handles
        // (that is, only the one we've connected) which will call the
        // callback function that has been connected to handle keyboard events.
        while (!myObject.letsexit) {
            base::Timer.update ();
            input::manager::update();
        }
        
        return 0;
    }

} theApp;
