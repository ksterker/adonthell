#ifndef GUI_TEXTBOX_H
#define GUI_TEXTBOX_H

#include "label.h"
#include "base/base.h"
namespace gui
{
	class textbox:public label
	{
	protected:
		int insertpos;
		int lastblink;
		bool hasfocus;
	public:
		textbox(int width, int height):label(width,height),insertpos(0),lastblink(::base::Timer.current_time()),hasfocus(false) {}
		textbox(const char* bgfile):label(bgfile),insertpos(0),lastblink(::base::Timer.current_time()),hasfocus(false) {}
		
		virtual void draw(int x, int y, gfx::surface* s);
		virtual bool keyup(input::keyboard_event & k);
		virtual bool keydown(input::keyboard_event & k); 
//Not till we can handle mouse events
//		virtual bool mouseup(SDL_MouseButtonEvent & m);
//		virtual bool mousedown(SDL_MouseButtonEvent & m);
//		virtual bool mousemove(SDL_MouseMotionEvent & m) { return false; }
		//accept the focus if it is given to us
		virtual bool focus() {hasfocus = true; return true;}
		virtual void unfocus() {hasfocus = false;}

	};
};


#endif//GUI_TEXTBOX_H
