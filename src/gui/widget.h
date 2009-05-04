#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include "gfx/gfx.h"
#include "gfx/surface_cacher.h"
#include "input/listener.h"
#include "gui/base.h"

namespace gui
{
	class widget:public base
	{
	protected:
		const gfx::surface * bg;
        /// the input handler
        input::listener * listener;
	public:
		widget(int width, int height):bg(NULL),listener(NULL) {setSize(width, height);}
		widget(const char* bgfile) {bg = gfx::surfaces->get_surface_only(bgfile); listener=NULL; w=bg->length(); h=bg->height();}
		~widget(){gfx::surfaces->free_surface(bg);delete listener;};
		virtual void draw(int x, int y, gfx::surface * s);
        
        /**
         * @name Input handling
         */
        //@{
        /**
         * Set an input listener so this widget can recieved keyboard input. Any
         * previously set input listeners are deleted.
         * @param lstnr the input listener to set.
         */
        void widget::set_listener (input::listener *lstnr);

        /**
         * Get the input listener associated to that widget, if any.
         * @return this widgets input listener.
         */
        input::listener * widget::get_listener () const;
        
        /**
         * Callback executed whenever a key on the keyboard has been pushed.
         * @param evt the key press event
         * @return whether the event was consumed or not.
         */
        bool on_keyboard_event (input::keyboard_event * evt);
        //@}
	};
};

#endif//GUI_WIDGET_H

