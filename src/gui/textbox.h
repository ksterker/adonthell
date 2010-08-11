#ifndef GUI_TEXTBOX_H
#define GUI_TEXTBOX_H

#include "gui/label.h"
#include "input/manager.h"

namespace gui
{
    /**
     * A one line text input widget.
     */
	class textbox : public label
	{
	protected:
		int insertpos;
		bool hasfocus;
	public:
		textbox(const u_int16 & width, const u_int16 & height):label(width,height),insertpos(0),hasfocus(false) {}
		textbox(const std::string & name):label(name),insertpos(0),hasfocus(false) {}
		
        /** 
         * Draw the object on the %screen.
         * 
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL,         
         * draw on the screen.
         */		
        virtual void draw(const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da = NULL, gfx::surface *target = NULL) const;
		virtual bool keyup(input::keyboard_event & k);
		virtual bool keydown(input::keyboard_event & k); 
        virtual bool input(input::keyboard_event & k); 

//Not till we can handle mouse events
//		virtual bool mouseup(SDL_MouseButtonEvent & m);
//		virtual bool mousedown(SDL_MouseButtonEvent & m);
//		virtual bool mousemove(SDL_MouseMotionEvent & m) { return false; }
		//accept the focus if it is given to us
		virtual bool focus() {hasfocus = true; input::manager::enable_unicode(true); return true;}
		virtual void unfocus() {hasfocus = false; input::manager::enable_unicode(false); }
	};
};

#endif//GUI_TEXTBOX_H
