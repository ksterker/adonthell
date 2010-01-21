#include "gui/font.h"
#include "base/base.h"
#include "base/endians.h"
#include "base/utf8.h"
#include <string>
using std::string;
#include <iostream>
using std::cout;
#include "base/base.h"
#include <ctype.h>

namespace gui
{
	FT_Library library;

	void font::ref(bool addref)
	{
		static int refcount = 0;
		if (addref)
		{
			if (!refcount)
			{
				if (error = FT_Init_FreeType(&library))
					cout << "Unable to initialize the freetype library\n";
				else
					refcount++;
			}
			else
				refcount++;
		}
		else
		{
			refcount--;
			if (!refcount)
				FT_Done_FreeType(library);
		}
	}
			
	font::font(const char* path, int size)
	{
		//check for the path
		const char* defont = "gfx/gui/Vera.ttf";
		if (!path)
			path = defont;
		
		std::string fullpath (path); 
		base::Paths.find_in_path (fullpath);
		//make sure the library is initialized
		error = 0;
		ref(true);
		if (error = FT_New_Face(library, fullpath.c_str(), 0, &face))
			cout << "Unable to load font " << path << "\n";
		else
			setSize(size);
		color = 0xffffffff; //set to white
	}
	font::~font()
	{
		ref(false);
	}

	void font::setSize(int size)
	{
		fontsize = size;
		if (error = FT_Set_Char_Size(face, 0, fontsize*64, 0, 0))
			cout << "Unable to set the size of the font\n";
	}

	void font::render(const string& s, int x, int y, gfx::surface* surf)
	{
		surf->lock();
		string::const_iterator i;
		for (i = s.begin(); i != s.end(); /* nothing */)
		{
            u_int32 chr = base::utf8::to_utf32 (s, i);
			if (error = FT_Load_Char(face, chr, FT_LOAD_RENDER))
				cout << "Unable to load the glyph for character '" << chr << "'\n";
			else
			{
				draw_glyph(x+face->glyph->bitmap_left, 
						   y-face->glyph->bitmap_top, 
						   &face->glyph->bitmap, surf);	
				x += face->glyph->advance.x >> 6;
				y += face->glyph->advance.y >> 6;
			}
		}
		surf->unlock();
	}

	void font::draw_glyph(int x, int y, FT_Bitmap* bmp, gfx::surface* s)
	{
		int i, j;
		//crop it
		int minx = x < 0 ? -x : 0;
		int maxx = x + bmp->width > s->length() ? s->length() - x : bmp->width;
		int miny = y < 0 ? -y : 0;
		int maxy = y + bmp->rows > s->height() ? s->height() - y : bmp->rows;
		unsigned char* data = bmp->buffer + miny*bmp->pitch + minx;
		for (j = miny; j < maxy; j++)
		{
			for (i = minx; i < maxx; i++)
			{
				union {
					u_int8 b[4];
					u_int32 c;
				} p1, p2;
				s->unmap_color(s->get_pix(x + i, y + j),p1.b[0], p1.b[1], p1.b[2], p1.b[3]);
				s->unmap_color(color, p2.b[0],p2.b[1],p2.b[2],p2.b[3]);
				
				if (*data)
				{
					float a2 = *data/255.0;
					//use an "over" compositing function
					p1.b[0] = (int)(p1.b[0]*(1-a2) + p2.b[0]*a2);
					p1.b[1] = (int)(p1.b[1]*(1-a2) + p2.b[1]*a2);
					p1.b[2] = (int)(p1.b[2]*(1-a2) + p2.b[2]*a2);
					p1.b[3] = (int)(p1.b[3]*(1-a2) + *data*a2);
				}
				s->put_pix(x + i, y + j, s->map_color(p1.b[0], p1.b[1], p1.b[2], p1.b[3]));
				data++;
			}
			data += bmp->pitch;
			data -= maxx-minx;
		}
	}
	void font::getMultilineSize(const string& s, int maxwidth, vector<textsize>& ts, int & w, int & h)
	{
		maxwidth <<= 6;
		int maxw;
		int th = 0;
		int tw = 0;
		int sw = 0;
		string::const_iterator i, lastspace = s.end();
		w = 0;
		h = fontsize << 6;
		int maxdrop = 0;
        for (i = s.begin(); i != s.end(); /* nothing */)
        {
            u_int32 chr = base::utf8::to_utf32 (s, i);
            if (error = FT_Load_Char(face, chr, FT_LOAD_DEFAULT))
                cout << "Unable to load the glyph for character '" << chr << "'\n";
			else
			{
				if (isspace(*i))
				{
					tw = w;
					lastspace = i;
					sw = face->glyph->advance.x;
				}
				w += face->glyph->advance.x;
				h += face->glyph->advance.y;
				if (w > maxwidth)
				{
					ts.push_back(textsize(tw >> 6, h >> 6, lastspace - s.begin()));
					if (maxwidth < tw)
						maxwidth = tw;
					w = w - tw - sw; //subtract the section, as well as the space
					th += fontsize << 6;
					maxdrop = 0; //only needs accounted for in the last line
				}

				if (maxdrop < face->glyph->metrics.height - face->glyph->metrics.horiBearingY)
					maxdrop = face->glyph->metrics.height - face->glyph->metrics.horiBearingY;
			}
		}
		ts.push_back(textsize(tw >> 6, h >> 6, s.end() - s.begin()));
		h += maxdrop;
		h += th;
		w = maxwidth;
		h >>= 6;
		w >>= 6;
		
	}
	//TODO: make this call getMultilineSize
	void font::getSize(const string& s, int& w, int& h)
	{
		string::const_iterator i;
		w = 0;
		h = fontsize << 6;
		int maxdrop = 0;
        for (i = s.begin(); i != s.end(); /* nothing */)
        {
            u_int32 chr = base::utf8::to_utf32 (s, i);
            if (error = FT_Load_Char(face, chr, FT_LOAD_DEFAULT))
                cout << "Unable to load the glyph for character '" << chr << "'\n";
			else
			{
				w += face->glyph->advance.x;
				h += face->glyph->advance.y;
				if (maxdrop < face->glyph->metrics.height - face->glyph->metrics.horiBearingY)
					maxdrop = face->glyph->metrics.height - face->glyph->metrics.horiBearingY;
			}
		}
		h += maxdrop;
		h >>= 6;
		w >>= 6;
	}


};
		
