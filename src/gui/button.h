#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H
#include "input/input.h"
#include "base/callback.h"
#include "label.h"
namespace gui
{
	//typedef void (*clickevent)(bool down, void* arg);
	typedef ::base::functor_2<bool, void * > * clickevent;
	class button:public label
	{
	protected:
		bool clicked;
		clickevent clickit;
		input::keyboard_event::key_type downkey;
		int clickpush;
	public:
		button(const u_int16 & width, const u_int16 & height, clickevent c, void* a=NULL, int o=5):label(width, height),clicked(false),clickit(c),clickpush(o),arg(a) { set_center (true, true); }
		button(const std::string & name, clickevent c, void* a=NULL, int o=5):label(name),clicked(false),clickit(c),clickpush(o),arg(a) { set_center (true, true); }
		~button() 
        {
            if(clickit) delete clickit;
        }

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
        
		virtual bool keyup(input::keyboard_event& k);
		virtual bool keydown(input::keyboard_event&k);
//not till we get a mouse_event class
//		virtual bool mouseup(SDL_MouseButtonEvent & m);
//		virtual bool mousedown(SDL_MouseButtonEvent & m);
		virtual bool focus() {return true;}
		virtual void unfocus() {clicked = false;}

		virtual void activate() {(*clickit)(clicked, arg);}
		//so we can change the argument later
		void* arg;
	};
};


#endif//GUI_LABEL_H
