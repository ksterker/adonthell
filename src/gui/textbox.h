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
 * @file   gui/textbox.h
 * @author Rian Shelley
 * @brief  Defines the textbox class.
 */

#ifndef GUI_TEXTBOX_H
#define GUI_TEXTBOX_H

#include "gui/label.h"

namespace gui
{
    /**
     * A cursor representing the position of text input.
     */
    class cursor
    {
    public:
        /**
         * Create the cursor.
         */
        cursor();

        /**
         * Update the state of the cursor. (Visible or not).
         * @param reset true to force cursor visible.
         */
        bool blink(const bool & reset = false);

    private:
        /// timestamp for a blinking cursor
        u_int32 LastBlink;
    };

    /**
     * A widget for text input. Text typed
     * by the user will be converted into
     * UTF-8 format.
     */
	class textbox : public label
	{
	public:
		/**
		 * Create a input field of the given size.
		 * @param width the width of the widget.
		 * @param height the height of the widget.
		 */
		textbox(const u_int16 & width, const u_int16 & height)
		: label(width, height), InsertPos(0), HasFocus(false)
		{
            Cursor = new cursor();
		}

		/**
		 * Create a input field of the given style.
		 * @param style the look of the widget.
		 */
		textbox(const std::string & style)
		: label(style), InsertPos(0), HasFocus(false)
		{
		    Cursor = new cursor();
		}
		
		/**
		 * Clean up.
		 */
		~textbox()
		{
		    delete Cursor;
		}

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

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::textbox);
#endif

	protected:
		/**
         * @name Keyboard Callbacks.
         *
         * Called when a keystroke happens. returns whether or not the
         * object used the keystroke
         */
        //@{
        /**
         * Called when a key has been released by the user.
         * @param k the keyboard event.
         * @return true if the event was consumed, false otherwise.
         */
		virtual bool keyup(input::keyboard_event & k);

        /**
         * Called when a key has been pressed by the user.
         * @param k the keyboard event.
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool keydown(input::keyboard_event & k);

        /**
         * Called when text has been input by the user.
         * @param k the keyboard event.
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool input(input::keyboard_event & k);
        //@}

		//Not till we can handle mouse events
		//		virtual bool mouseup(SDL_MouseButtonEvent & m);
		//		virtual bool mousedown(SDL_MouseButtonEvent & m);
		//		virtual bool mousemove(SDL_MouseMotionEvent & m) { return false; }

        /**
         * @name Focus Handling
         */
        //@{
		/**
         * Called when object is offered the keyboard focus. Returns true
		 * to accept it.
         * @return true to accept focus, false to decline.
		 */
		virtual bool focus()
		{
			HasFocus = true;
			input::manager::enable_unicode(true);
			return true;
		}

        /**
         * Notification that the object will no longer receive keyboard
         * events
         */
		virtual void unfocus()
		{
			HasFocus = false;
			input::manager::enable_unicode(false);
		}
        //@}

		/// cursor position
		int InsertPos;
		/// whether widget has the input focus
		bool HasFocus;
		/// the cursor for text input
		cursor *Cursor;
	};
};

#endif//GUI_TEXTBOX_H
