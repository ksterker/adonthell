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
	inline void vertLine(int x, int y1, int y2, u_int32 c, gfx::surface*s)
	{
		if (x > 0 && x < s->length())
		{
			int ymin = y1 < 0 ? 0 : y1;
			int ymax = y2 > s->height() ? s->height() : y2;
			s->lock();
			for(;ymin < ymax;ymin++)
				s->put_pix(x, ymin, c);
			s->unlock();
		}
	}
	inline void horizLine(int x1, int x2, int y, u_int32 c, gfx::surface*s)
	{
		if (y > 0 && y < s->height())
		{
			int xmin = x1 < 0 ? 0 : x1;
			int xmax = x2 > s->length() ? s->length() : x2;
			s->lock();
			for(;xmin < xmax;xmin++)
				s->put_pix(xmin, y, c);
			s->lock();
		}
	}
	inline void box(int x, int y, int w, int h, u_int32 c, gfx::surface*s)
	{
		vertLine(x, y, y + h, c, s);
		vertLine(x+w-1, y, y + h, c, s);
		horizLine(x, x + w, y, c, s);
		horizLine(x, x + w, y+ h-1, c, s);
	}
	inline void border(int x, int y, int w, int h, gfx::surface*s)
	{
		x-=5;y-=5;w+=10;h+=10;
		box(x, y, w, h, 0xff000040, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xff000080, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xff0000b0, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xff000080, s);
		x++;y++;w-=2;h-=2;
		box(x, y, w, h, 0xff000040, s);
	}

};

#endif//DRAW_H
