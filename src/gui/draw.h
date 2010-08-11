/*
 * this file contains basic functions for doing drawing
 * TODO: implement alpha blending
 * TODO: remove assumption of 32 bit color
 */

#ifndef DRAW_H
#define DRAW_H

#include "gfx/surface.h"

namespace gui
{
	inline void box(int x, int y, int w, int h, u_int32 c, gfx::surface*s)
	{
        gfx::drawing_area da (0, 0, s->length(), s->height());
        s->draw_line (x, y, x, y + h, c, &da);
        s->draw_line (x + w, y, x + w, y + h, c, &da);
        s->draw_line (x, y, x + w, y, c, &da);
        s->draw_line (x, y + h, x + w, y + h, c, &da);
	}
	inline void border(int x, int y, int w, int h, gfx::surface*s)
	{
		x-=5;y-=5;w+=10;h+=10;
		box(x, y, w, h, 0xff685510, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xffc0a539, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xffeae091, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xffe4d360, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xffa48b28, s);
	}

};

#endif//DRAW_H
