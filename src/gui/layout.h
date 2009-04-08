#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include "gui/base.h"
#include "input/input.h"
#include <vector>

#ifndef SWIG
using std::vector;
#endif


namespace gui
{
	struct guirect
	{
		int x, y, w, h;
	};
	struct layoutchild
	{
	public:
		base* c;
		guirect pos;
		layoutchild(base*b, struct guirect& r):c(b),pos(r) {}
	};

	enum fadetype {NONE, PLAIN, FADE, LEFT, RIGHT, BOTTOM, TOP};
	class layout: public base
	{
	protected:
		vector<layoutchild> children;
		float dx, dy;
		int which;
		fadetype fading;
		bool showing;
		u_int32 ti;
		bool focused;
		//which way to move the focus. 
		virtual bool moveright();
		virtual bool moveleft();
		virtual bool moveup();
		virtual bool movedown();
		//void dofade(int x, int y, const SDL_Surface* s);
		bool mousestate[3];
	public:
		layout():which(0),dx(0),dy(0),fading(NONE),focused(false),hasborder(0) {selhilite=false;mousestate[0]=mousestate[1]=mousestate[2]=false;}

		virtual void draw(int x, int y, gfx::surface* s);
		virtual bool keyup(input::keyboard_event & k);
		virtual bool keydown(input::keyboard_event & k);
//pending mouse support
//		virtual bool mouseup(SDL_MouseButtonEvent & m);
//		virtual bool mousedown(SDL_MouseButtonEvent & m);
//		virtual bool mousemove(SDL_MouseMotionEvent & m) { return false; }

		virtual bool focus(); 
		virtual void unfocus() {if (children.size()) children[which].c->unfocus(); focused = false;}

		virtual void setSize(int width, int height) {w = width; h = height;}
		
		void addchild(base& c, int x, int y); 
		void removechild(base& c);
		void hide(fadetype f=PLAIN);
		void show(fadetype f=PLAIN);
		bool shown() { return visible;}
		u_int32 hasborder; //could stick the color in here
	};
};

#endif//GUI_BASE_H
