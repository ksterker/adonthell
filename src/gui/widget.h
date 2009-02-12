#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include "gfx/gfx.h"
#include "gfx/surface_cacher.h"
#include "gui/base.h"

namespace gui
{
	class widget:public base
	{
	protected:
		const gfx::surface * bg;
	public:
		widget(int width, int height):bg(NULL) {setSize(width, height);}
		widget(const char* bgfile) {bg = gfx::surfaces->get_surface_only(bgfile); w=bg->length(); h=bg->height();}
		~widget(){gfx::surfaces->free_surface(bg);};
		virtual void draw(int x, int y, gfx::surface * s);
	};
};

#endif//GUI_WIDGET_H

