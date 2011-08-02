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
 * @file   gui/label.h
 * @author Rian Shelley
 * @brief  Defines the label class.
 */

#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "gui/widget.h"
#include "gui/font.h"

namespace gui
{
	/**
	 * A widget for displaying text, either
	 * a single or multiple lines.
	 *
	 * A label can neither receive focus nor
	 * react to user input.
	 */
	class label : public widget
	{
	public:
		/**
		 * Create a label of the given size. Initially the label text
		 * will not be centered, single line and use a default font.
		 * @param width the width of the label.
		 * @param height the height of the label.
		 */
		label(const u_int16 & width, const u_int16 & height)
		: widget (width, height), CenterX(false), CenterY(false), Multiline(false), AutoHeight(false), Ox(0), Oy(0)
        {
            Font = new font(); 
        }

		/**
		 * Create a label with the given look. Initially the label text
		 * will not be centered, single line and use a default font.
		 * @param style the look of the label.
		 */
		label(const std::string & style)
		: widget(style), CenterX(false), CenterY(false), Multiline(false), AutoHeight(false), Ox(0), Oy(0)
        {
            Font = new font(); 
        }
        
		/**
		 * Delete label.
		 */
        virtual ~label()
        {
            delete Font;
        }
        
        /**
         * @name Label content
         */
        //@{
		/**
		 * Set the text displayed by the label.
		 * It should be encoded in UTF-8 format.
		 *
		 * @param s the text to display on the label.
		 */
		void set_string (const std::string & s)
        { 
            Text = s;

            reheight();
        }
        
		/**
		 * Get the text displayed by the label.
		 * It is (usually) encoded in UTF-8 format.
		 *
		 * @return the text contained in the label.
		 */
		const std::string & get_string() const
        { 
            return Text; 
        }
        //@}
        
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
        
		/**
         * @name Text attributes.
         *
         * Change or query the appearance of
         * text in the widget.
         */
		//@{
		/**
		 * Set centering of text in the label.
		 * @param cx center text horizontally
		 * @param cy center text vertically
		 */
        void set_center (const bool & cx, const bool & cy ) 
        {
            CenterX = cx;
            CenterY = cy; 
        }
        
        /**
         * Is text centered horizontally?
         * @return true if this is the case, false otherwise.
         */
        bool center_x() const { return CenterX; }

        /**
         * Is text centered vertically?
         * @return true if this is the case, false otherwise.
         */
        bool center_y() const { return CenterY; }
        
        /**
         * Turn on wrapping of text across multiple lines.
         * @param m true to turn on line wrapping.
         */
		void set_multiline (const bool & m, const bool & grow = true)
		{
			Multiline = m;

			// enable only for multiline label
			if (grow) AutoHeight = m;
			else AutoHeight = false;

			// update label size
			reheight();
		}
        
        /**
         * Is text wrapped across multiple lines?
         * @return true if this is the case, false otherwise.
         */
		bool multiline () const { return Multiline; }

		/**
		 * Set the current text color.
		 * @param c the text color.
		 */
		void set_color(const u_int32 & c) { Font->set_color(c); }

		/**
		 * Get the current text color.
		 * @return the text color.
		 */
		u_int32 get_color() const { return Font->color(); }
		//@}

        font *get_font () const { return Font; }

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::label);
#endif

    protected:
		/**
		 * Update the label height, if necessary.
		 */
		void reheight();

		/**
		 *
		 */
	    void set_offset (const u_int32 & x, const u_int32 & y) { Ox = x; Oy = y; }

	    /**
	     *
	     */
	    void draw_text (const s_int16 & x, const s_int16 & y, const gfx::drawing_area *da, gfx::surface *target) const;

        /// whether to center the text horizontally
        bool CenterX;
        /// whether to center the text vertically
        bool CenterY;
        /// whether the text should wrap across multiple lines
        bool Multiline;
        /// whether the label height should grow to accommodate the text
        bool AutoHeight;
        /// the text
        std::string Text;
        /// the font
		font *Font;
	    /// text draw offset x
	    u_int32 Ox;
	    /// text draw offset y
	    u_int32 Oy;
	};
};


#endif//GUI_LABEL_H
