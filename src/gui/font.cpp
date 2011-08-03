/*
 Copyright (C) 2008 Rian Shelley
 Part of the Adonthell Project http://adonthell.linuxgames.com

 Adonthell is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 Adonthell is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Adonthell; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * @file   gui/font.cpp
 * @author Rian Shelley
 * @brief  Implements the font class.
 */

#include <string>
#include <sstream>
#include <ctype.h>

#include "gui/font.h"
#include "gui/fontcache.h"

#include "base/base.h"
#include "base/endians.h"
#include "base/logging.h"
#include "base/utf8.h"
#include "gfx/gfx.h"

using std::string;

namespace gui
{
	FT_Library library;

	font_cache font::FontCache;

	// ctor
	font::font(const char* path, int size)
	{
		//check for the path
		const char* defont = "gfx/gui/Vera.ttf";
		if (!path)
		{
			path = defont;
		}
		
		std::string fullpath (path); 
		if (!base::Paths().find_in_path (fullpath))
		{
		    Error = true;
            LOG(ERROR) << logging::indent() << "Unable to find font '" << path << "'";
            return;
		}

		// make sure the library is initialized
		ref (true);

		// load font
		if (Error = FT_New_Face(library, fullpath.c_str(), 0, &Face))
		{
			LOG(ERROR) << logging::indent() << "Unable to load font '" << path << "'";
		}
        else
		{
			set_size (size);
			Name = path;
		}

		Color = 0xffffffff; //set to white
	}

	font::~font()
	{
		ref(false);
	}

    void font::ref(bool addref)
    {
        static int refcount = 0;
        if (addref)
        {
            if (!refcount)
            {
                if (Error = FT_Init_FreeType(&library))
                {
                    LOG(ERROR) << logging::indent() << "Unable to initialize the freetype library";
                    return;
                }
            }
            refcount++;
        }
        else
        {
            refcount--;
            if (!refcount)
            {
                FT_Done_FreeType(library);
            }
        }
    }

	void font::set_size(int size)
	{
		FontSize = size;
		if (Error = FT_Set_Pixel_Sizes (Face, 0, FontSize))
		{
			LOG(ERROR) << logging::indent() << "Unable to set the size of the font";
		}
	}

	void font::draw_text(const string& s, const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface* target)
	{
	    s_int16 ox = x;

		string::const_iterator i;
		for (i = s.begin(); i != s.end(); /* nothing */)
		{
            u_int32 chr = base::utf8::to_utf32 (s, i);
            const glyph_info *gi = FontCache.get (chr, this);

            gi->Foreground->s->draw(ox + gi->x, y + gi->y, da, target);
            ox += gi->length;
		}
	}

    void font::draw_shadow(const string& s, const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface* target)
    {
        s_int16 ox = x;

        string::const_iterator i;
        for (i = s.begin(); i != s.end(); /* nothing */)
        {
            u_int32 chr = base::utf8::to_utf32 (s, i);
            const glyph_info *gi = FontCache.get (chr, this);

            gi->Background->s->draw(ox + gi->x - 3, y + gi->y - 3, da, target);
            ox += gi->length;
        }
    }

    gui::glyph_info *font::create_glyph (const u_int32 & chr)
    {
        // new glyph structure
        glyph_info *gi = new glyph_info();

        if (Error = FT_Load_Char(Face, chr, FT_LOAD_RENDER))
        {
            LOG(ERROR) << logging::indent() << "Unable to load the glyph for character '" << chr << "'";

            gi->length = 0;
            gi->height = 0;
            gi->drop = 0;
            gi->x = 0;
            gi->y = 0;
        }
        else
        {
            std::stringstream name (std::ios::out);
            gui::color fg_color, bg_color;
            bg_color.i = 0;

            // the foreground glyph is always unique, so just create it
            gfx::surface *foreground = gfx::create_surface();

            foreground->set_alpha (255, true);
            foreground->resize (Face->glyph->bitmap.width, Face->glyph->bitmap.rows);
            foreground->unmap_color (Color, fg_color.b[2], fg_color.b[1], fg_color.b[0], fg_color.b[3]);
            // TODO: big endian? foreground->unmap_color (Color, fg_color.b[0], fg_color.b[1], fg_color.b[2], fg_color.b[3]);
            foreground->fillrect (0, 0, foreground->length(), foreground->height(), foreground->map_color (fg_color.b[0], fg_color.b[1], fg_color.b[2], 0));

            // render foreground image
            render_glyph(0, 0, &Face->glyph->bitmap, foreground, fg_color);

            // add it to surface cache
            name << std::hex << chr << "_" << fg_color.i << "_" << std::dec << Name << "_" << FontSize;
            gi->Foreground = gfx::surfaces->add_surface_mem (name.str(), foreground);

            // determine color to use for the background
            float intensity = fg_color.b[0]*.3 + fg_color.b[1]*0.59 + fg_color.b[2]*0.11;
            bg_color.b[0] = bg_color.b[1] = bg_color.b[2] = intensity < 128.0 ? 0xff : 0x00;

            // a suitable background glyph may already exist, so try to get it
            name.str("b_");
            name << std::hex << chr << "_" << bg_color.i << "_" << std::dec << Name << "_" << FontSize;

            gi->Background = gfx::surfaces->get_surface_mem (name.str());
            if (gi->Background == NULL)
            {
                gfx::surface *background = gfx::create_surface();

                background->set_alpha (255, true);
                background->resize (Face->glyph->bitmap.width + 6, Face->glyph->bitmap.rows + 6);
                background->fillrect(0, 0, background->length(), background->height(), background->map_color (bg_color.b[0], bg_color.b[1], bg_color.b[2], 0));

                // render background image
                render_glyph(3, 3, &Face->glyph->bitmap, background, bg_color);

                // apply gaussian blur
                background->blur();

                // store in cache
                gi->Background = gfx::surfaces->add_surface_mem (name.str(), background);
            }

            gi->length = Face->glyph->advance.x >> 6;
            gi->height = Face->glyph->advance.y >> 6;
            gi->drop = (Face->glyph->metrics.height - Face->glyph->metrics.horiBearingY) >> 6;
            gi->x = Face->glyph->bitmap_left;
            gi->y = -Face->glyph->bitmap_top;
        }

        return gi;
    }

    // render a single glyph to a surface
    void font::render_glyph(int x, int y, FT_Bitmap* bmp, gfx::surface *surf, gui::color & col)
    {
        gui::color p1;

        surf->lock();

        unsigned char* data = bmp->buffer;
        for (int j = 0; j < bmp->rows; j++)
        {
            for (int i = 0; i < bmp->width; i++)
            {
                if (*data)
                {
                    float a2 = *data / 255.0;

                    p1.b[0] = (u_int8) (col.b[0] * a2);
                    p1.b[1] = (u_int8) (col.b[1] * a2);
                    p1.b[2] = (u_int8) (col.b[2] * a2);
                    p1.b[3] = *data;

                    surf->put_pix (x + i, y + j, surf->map_color (p1.b[0], p1.b[1], p1.b[2], p1.b[3]));
                }

                data++;
            }

            data += bmp->pitch;
            data -= bmp->width;
        }

        surf->unlock();
    }

    // get text size with line wrapping enabled
	void font::get_text_size(const string& s, const u_int16 & max_line_width, vector<textsize>& ts, u_int16 & w, u_int16 & h)
	{
        w = 0;
        h = FontSize;

		int line_width = 0;
		int space_width = 0;
        int max_width = 0;
		int max_drop = 0;

		string::const_iterator i, last_space = s.end();
        for (i = s.begin(); i != s.end(); /* nothing */)
        {
            u_int32 chr = base::utf8::to_utf32 (s, i);
            const glyph_info *gi = FontCache.get (chr, this);

            if (isspace(*i))
            {
                line_width = w;
                last_space = i;
                space_width = gi->length;
            }

            w += gi->length;

            // exceeding maximum line width or manual line break?
            if (w > max_line_width || *i == '\n')
            {
                // wrap at last whitespace character
                ts.push_back(textsize(line_width, FontSize, last_space - s.begin()));

                // calculate line widths
                if (max_width < line_width) max_width = line_width;

                // subtract the section, as well as the space
                w = w - line_width - space_width;

                // update height
                h += FontSize;

                // only needs accounted for in the last line
                max_drop = 0;
            }

            // find out how much the text extends into the next line
            if (max_drop < gi->drop)
                max_drop = gi->drop;
		}

        // add last line
		ts.push_back(textsize(w, FontSize, s.end() - s.begin()));

		// final line height
		h += max_drop;

		// final text width
		w = max_width > w ? max_width : w;
	}

	//get text size without line wrapping
	void font::get_text_size(const string& s, u_int32 & w, u_int32 & h)
	{
		w = 0;
		h = FontSize;

		int max_drop = 0;
		string::const_iterator i;

        for (i = s.begin(); i != s.end(); /* nothing */)
        {
            u_int32 chr = base::utf8::to_utf32 (s, i);
            const glyph_info *gi = FontCache.get (chr, this);

            w += gi->length;

            // find out how much the text extends into the next line
            if (max_drop < gi->drop)
                max_drop = gi->drop;
        }

        h += max_drop;
	}
};
		
