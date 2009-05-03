#include "input/manager.h"
#include "gui/widget.h"

namespace gui
{
	void widget::draw(int x, int y, gfx::surface* s)
	{
		if (bg)
		{
			bg->draw(x, y, NULL, s);
		}
	}
    
    // set input listener
    void widget::set_listener (input::listener *lstnr) 
    {
        if (listener) 
        {
            input::manager::remove (listener);
            delete listener;
        }
        
        listener = lstnr;
        
        if (listener) 
        {
            listener->connect_keyboard_function (::base::make_functor_ret(*this, &widget::on_keyboard_event));
        }
    }
    
    // get input listener
    input::listener * widget::get_listener () const 
    {
        return listener;
    }
    
    // handle keyboard events
    bool widget::on_keyboard_event (input::keyboard_event * evt) 
    {
        if (evt->type() == input::keyboard_event::KEY_PUSHED) 
        {
            return keydown (*evt);
        } 
        else 
        {
            return keyup (*evt);
        }
        
        return false;
    } 
};
