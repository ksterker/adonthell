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
 * @file   gui/option.h
 * @author Rian Shelley
 * @brief  Defines the option class.
 */

#ifndef GUI_OPTION_H
#define GUI_OPTION_H

#include "button.h"

namespace gui
{
	/**
	 * A selectable widget that remembers its
	 * selection state. Can be used to represent
	 * toggle buttons or check boxes.
	 */
	class option: public button
	{
	public:
		/**
		 * Create a option widget of the given size.
		 * @param width the width of the widget.
		 * @param height the height of the widget.
		 */
		option(const u_int16 & width, const u_int16 & height)
		: button(width, height), State(false)
		{ }

        /**
         * Create option widget from the given data file.
         * Size will be set to the background size.
         * @param style filename of widget decoration.
         */
		option(const std::string & style)
		: button(style), State(false)
		{ }

		/**
		 * @name Selection state
		 */
		//@{
		/**
		 * Set selection state of option widget. This
		 * does not trigger an event.
		 * @param s true to select, false otherwise.
		 */
		void set_state(const bool & s) { Clicked = s; }

		/**
		 * Get selection state of option widget.
		 * @return true if selected, false otherwise.
		 */
		bool get_state() const { return Clicked; }
		//@}

		/**
		 * Called when the widget has been activated.
		 * If present, executes the callback with the
		 * selection state and the user defined data.
		 */
		virtual void activate();

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::option);
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
         * Notification that the object will no longer receive keyboard
         * events. Unlike a button, keep the selection state when focus
         * is lost.
         */
		virtual void unfocus() { }
		//@}

		//waiting for mouse support
		/*
				virtual bool mouseup(SDL_MouseButtonEvent & m);
				virtual bool mousedown(SDL_MouseButtonEvent & m);
		// */

	private:
		/// state of option widget (selected or not)
		bool State;
	};
}


#endif//GUI_LABEL_H
