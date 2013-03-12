/*
 Copyright (C) 2008 Rian Shelley
 Copyright (C) 2011 Kai Sterker <kai.sterker@gmail.com>
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
 * @file   gui/font.h
 * @author Rian Shelley
 * @author Kai Sterker
 * @brief  Declares the font class.
 */

#ifndef FONT_H
#define FONT_H

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <adonthell/gfx/drawing_area.h>
#include <adonthell/gfx/surface_cacher.h>

#ifndef SWIG
using std::vector;
using std::string;
#endif

namespace gui
{
    /**
     * Structure to store a color in RGBA format
     */
    typedef union
    {
        u_int8 b[4];
        u_int32 i;
    } color;

    /**
     * Data for a single glyph, including graphical representations
     * for background (shadow) and actual character.
     */
    class glyph_info
    {
    public:
        /**
         * Constructor.
         */
        glyph_info() : Foreground(NULL), Background(NULL)
        {
        }

        /**
         * Destructor.
         */
        ~glyph_info()
        {
            delete Foreground;
            delete Background;
        }

        /// foreground of the glyph
        const gfx::surface_ref *Foreground;
        /// background of the glyph, for contrast
        const gfx::surface_ref *Background;

        /// start position of the glyph
        s_int32 x;
        /// start position of the glyph
        s_int32 y;
        /// size of the actual glyph
        u_int32 length;
        /// size of the actual glyph
        u_int32 height;
    };

    /**
     * Information about the size of single line of text
     * used when rendering multi-line text.
     */
	class textsize
	{
	public:
        textsize(int _w, int _h, int c): w(_w),h(_h),cpos(c) {}

        /// actual width of the line when rendered
        int w;
        /// actual height of the line when rendered
        int h;
        /// index in string where this line starts
		int cpos;
	};

	/**
	 * Represents a font with a certain size and color that can be used
	 * to render text onto surfaces.
	 */
	class font
	{
	public:
	    /**
	     * Create font for given TTF file and size.
	     * @param path path to a true type font.
	     * @param size the font size in pixels(?).
	     */
		font(const char* path=NULL, int size=24);

		/**
		 * Destroy font and related resources.
		 */
		~font();

		/**
		 * Draw a string of text with this font. This simply
		 * places each character of the string into one line,
		 * starting at the given offset. Combine with calls to
		 * draw_shadow and get_text_size to get better readable
		 * or multiline text.
		 *
		 * @param s the text to draw.
		 * @param x the x offset.
		 * @param y the y offset
		 * @param da the clipping rectangle
		 * @param target the render target.
		 */
	    void draw_text (const string& s, const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da = NULL, gfx::surface* target = NULL);

        /**
         * Call before draw_text to get a nice background for
         * the actual text.
         *
         * @param s the text to draw.
         * @param x the x offset.
         * @param y the y offset
         * @param da the clipping rectangle
         * @param target the render target.
         */
	    void draw_shadow (const string& s, const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da = NULL, gfx::surface* target = NULL);

	    /**
	     * Get the distance from baseline to baseline for the current font.
	     * @return the line height.
	     */
	    u_int32 get_line_height () const
	    {
	        return Face->size->metrics.height >> 6;
	    }

	    /**
	     * Calculate size of the given string when drawn with
	     * this font, without any line wrapping.
	     * @param s the text to measure.
	     * @param w reference that will be set to the width
	     * @param h reference that will be set to the height
	     */
        void get_text_size(const string& s, u_int32& w, u_int32 & h);

        /**
         * Calculate size of the given string when drawn with
         * this font, including word wrapping at whitespace.
         * @param s the text to measure.
         * @param max_line_width the maximum line width in pixels.
         * @param ts reference set to the individual lines the text was split.
         * @param w reference set to the actual width measured.
         * @param h reference set to the actual height measured.
         */
        void get_text_size(const string& s, const u_int16 & max_line_width, vector<textsize>& ts, u_int16 & w, u_int16 & h);

        /**
         * Check whether the font is ready for use.
         * @return false if any kind of error occurred.
         */
		bool is_valid() const { return !Error; }

		/**
		 * Set the font size.
		 * @param size the new font size.
		 */
		void set_size(int size); 

		/**
		 * Get the current font size.
		 * @return the font size.
		 */
		int size() const { return FontSize; }

		/**
		 * Set the font color.
		 * @param the new color in RGBA format.
		 */
        void set_color(u_int32 c) {Color = c;}

        /**
         * Get the font color.
         * @return the font color.
         */
		u_int32 color() const {return Color;}

		/**
		 * Get the font name.
		 * @return the name passed to the constructor.
		 */
		std::string name() const { return Name; }

		/**
		 * Create information for the given glyph.
		 * @param chr the character in UTF32 format.
		 */
        gui::glyph_info* create_glyph (const u_int32 & chr);

	private:
        /**
         * Update reference count for freetype library usage.
         * @param addref true to increase refcount, false to decrease.
         */
        void ref(bool addref);

        /**
         * Renders the given glyph in the given color onto the given surface.
         * @param x offset in x direction
         * @param y offset in y direction
         * @param bmp the glyph bitmap
         * @param surf the surface to render on
         * @param c the color to use for rendering.
         */
        void render_glyph(int x, int y, FT_Bitmap* bmp, gfx::surface* surf, gui::color & c);

        /// a freetype font face
        FT_Face Face;
        /// state of the font
        int Error;
        /// color of the font
        u_int32 Color;
        /// size of the font
        int FontSize;
        /// name of the font
        std::string Name;
	};
}

#endif//FONT_H

