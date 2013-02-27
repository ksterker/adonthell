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
 * @file   gui/button.h
 * @author Rian Shelley
 * @brief  Defines the button class.
 */

#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "label.h"
#include "ui_event.h"

namespace gui
{
	/**
	 * A widget that can receive focus and react to
	 * user input. Otherwise it has the same functionality
	 * as a label. Button text is centered in x and y
	 * direction by default.
	 *
	 * With the proper style it becomes a button, but
	 * it can also be used to represent a selectable
	 * list item or a slot in an inventory.
	 */
	class button : public label
	{
	public:

		/**
		 * Create a button of the given size.
		 * @param width the width of the button.
		 * @param height the height of the button.
		 */
		button(const u_int16 & width, const u_int16 & height)
		: label (width, height), Clicked(false), DownKey(input::keyboard_event::UNKNOWN_KEY)
		{
			set_center (true, true);
		}

        /**
         * Create button from the given data file.
         * Size will be set to the background size.
         * @param style filename of widget decoration.
         */
		button(const std::string & style)
		: label (style), Clicked(false), DownKey(input::keyboard_event::UNKNOWN_KEY)
		{
			set_center (true, true);
		}

		/**
		 * Destroy the button and its associated resources.
		 */
		~button() 
        {
        }

		/**
		 * Create an event required to register a callback that
		 * reacts to button activation.
		 *
		 * @param user_data a user object that will be passed to
		 *      the callback when the button is activated.
		 * @return the ui_event for button activation.
		 */
		gui::ui_event *get_activate_event (void* user_data = NULL);

		/**
		 * Called when the widget has been activated.
		 * If present, executes the callback with the
		 * user defined data.
		 */
		virtual void activate();

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
        GET_TYPE_NAME_VIRTUAL(gui::button);
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
		virtual bool keyup(input::keyboard_event& k);

        /**
         * Called when a key has been pressed by the user.
         * @param k the keyboard event.
         * @return true if the event was consumed, false otherwise.
         */
		virtual bool keydown(input::keyboard_event& k);
		//@}

        /**
         * @name Focus Handling
         */
        //@{
		/**
         * Called when object is offered the keyboard focus. Returns true
		 * to accept it.
         * @return true to accept focus, false to decline.
		 */
		virtual bool focus() { return true; }

        /**
         * Notification that the object will no longer receive keyboard
         * events
         */
		virtual void unfocus() { Clicked = false; }
		//@}

		//not till we get a mouse_event class
		//		virtual bool mouseup(SDL_MouseButtonEvent & m);
		//		virtual bool mousedown(SDL_MouseButtonEvent & m);

		/// whether the button is currently being pressed
		bool Clicked;
		/// the key that caused the button to be pressed
		input::keyboard_event::key_type DownKey;
	};
}


#endif//GUI_BUTTON_H
