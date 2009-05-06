#include "gui/label.h"
#include "gfx/gfx.h"
#include <iostream>
using std::cout;

//const float sigma1[] = {.006,.061,.242,.383,.242,.061,.006};
//const float kernel[] ={.16,.16,.16,0,.16,.16,.16};
const float kernel[] ={.0,.26,.42,0.1,.42,.26,0};

typedef union {
	u_int8 c[4];
	u_int32 i;
} color;

void gaussianblur(gfx::surface* s)
{
	int i, j, k;
	
	s->lock();
	//make a copy of the data
	u_int32* data = new u_int32[s->length() * s->height()];
	k = 0;
	for (j = 0; j < s->height(); j++)
		for (i = 0; i < s->length(); i++)
			data[k++] = s->get_pix(i, j);

	//horizontal
	for (j = 0; j < s->height(); j++)
	{
		for (i = 0; i < s->length(); i++)
		{
			int kmin = i < 4? 3 - i : 0;
			int kmax = i > s->length() - 4? s->length() - i + 3 : 7;
			color c, r;
			r.i = 0;
			for (k = kmin; k < kmax; k++)
			{
				c.i = data[j * s->length() + i + (k - 3)];
				r.c[0] += c.c[0] * kernel[k];
				r.c[1] += c.c[1] * kernel[k];
				r.c[2] += c.c[2] * kernel[k];
				r.c[3] += c.c[3] * kernel[k];
			}
			s->put_pix(i, j, r.i);
		}
	}
	//another copy, smeared horizontally
	k = 0;
	for (j = 0; j < s->height(); j++)
		for (i = 0; i < s->length(); i++)
			data[k++] = s->get_pix(i, j);
	//vertical
	for (j = 0; j < s->height(); j++)
	{
		for (i = 0; i < s->length(); i++)
		{
			int kmin = j < 4? 3 - j : 0;
			int kmax = j > s->height() - 4? s->height() - j + 3 : 7;
			color c, r;
			r.i = 0;
			for (k = kmin; k < kmax; k++)
			{
				c.i = data[(j + (k - 3)) * s->length() + i];
				r.c[0] += c.c[0] * kernel[k];
				r.c[1] += c.c[1] * kernel[k];
				r.c[2] += c.c[2] * kernel[k];
				r.c[3] += c.c[3] * kernel[k];
			}
			s->put_pix(i, j, r.i);
		}
	}
	delete [] data;
	s->unlock();
}

namespace gui
{
	void label::draw(int x, int y, gfx::surface* s)
	{
		if (!cachevalid) //do we need to regenerate the cached image?
		{
            //find the needed size of the cache
			int nw, nh;
			f.getSize(txt, nw, nh);
			int mw = nw, mh = nh;
			//TODO: properly account for the case where both centering and 
			//multiline are enabled. 
			if (nw > w) nw = w; //clamp the size to the size of the widget
			if (nh > h) nh = h;
			if (!cached) //does the existing cache need cleared?
			{
				cached = gfx::create_surface();
				cached->set_alpha(255, true);
				cached->resize(w, h);
			}
			if (!cached)
				cout << "Unable to create label cache!\n";
			cached->fillrect(0, 0, w, h, cached->map_color (0, 0, 0, 0));
			//compute where to render it
			if (_multiline)
				rx = 0;
			else if (!offset && centerx) //if a manual offset has been set, centering doesnt matter
				rx = (w - nw)/2;
			else
				rx = -offset;
			if (centery)
				ry = (h + f.getSize())/2;
			else
				ry = f.getSize();
			//this effect tries to do a gaussian blur of the text behind it
			//the idea is to make the text itself easier to read
			//try to get a reasonable background color
			//*
			color old, n;
			old.i = f.getColor();
            s->unmap_color(old.i, n.c[0], n.c[1], n.c[2], n.c[3]); 
			float intensity = n.c[0]*.3 + n.c[1]*0.59 + n.c[2]*0.11;
			if (intensity < 128.0)
				n.i = cached->map_color(0xff,0xff,0xff,0xff);
			else
				n.i = cached->map_color(0,0,0,0xff);
			f.setColor(n.i);
            
			//render in a background color, to get contrast
			if (_multiline)
			{
				std::vector<gui::textsize> ts;
				int tw, th;
				f.getMultilineSize(txt, w, ts, tw, th);
				int i;
				int p = 0;
				int y = ry;
				for (i = 0; i < ts.size(); i++)
				{
					f.render(txt.substr(p, ts[i].cpos-p), rx, y, cached);
					y += ts[i].h;
					p = ts[i].cpos + 1;
				}
			}
			else
				f.render(txt, rx,ry, cached);
			
            f.setColor(old.i);
			//apply a gaussian blur to it
			gaussianblur(cached);			
			// */
			//render on top of the blur, with the normal color
			//
			unsigned char r, g, b, a;
			unsigned int oldcolor = f.getColor();
			s->unmap_color(f.getColor(), r, g, b, a);
			f.setColor(cached->map_color(r, g, b, a));

			if (_multiline)
			{
				std::vector<gui::textsize> ts;
				int tw, th;
				f.getMultilineSize(txt, w, ts, tw, th);
				int i;
				int p = 0;
				int y = ry;
				for (i = 0; i < ts.size(); i++)
				{
					f.render(txt.substr(p, ts[i].cpos-p), rx, y, cached);
					y += ts[i].h;
					p = ts[i].cpos + 1;
				}
			}
			else
				f.render(txt, rx,ry, cached);
			cachevalid = true;
			f.setColor(oldcolor);
		}

		widget::draw(x, y, s);
		cached->draw(x+px, y+py, NULL, s);
	}
	/*
	 * This function will change the height of the object based on the text
	 * It sets multiline to true
	 */
	void label::reheight()
	{
		multiline(true);
		vector<textsize> ts;
		int newwidth=0, newheight=0;
		f.getMultilineSize(txt, w, ts, newwidth, newheight);
		setSize(w, newheight);
	}
};

