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
 * @file gui/widget.h
 * @author Rian Shelley
 * @brief  Defines the surface global interface.
 */

#ifndef GUI_BASE_H
#define GUI_BASE_H

#include "gfx/gfx.h"
#include "gfx/drawable.h"
#include "gui/decoration.h"
#include "input/keyboard_event.h"

namespace gui
{
    /**
     * The base class for all user interface elements, both widgets and
     * layouts.
     */
	class base : public gfx::drawable
	{
	public:
        /**
         * Create a new GUI element. Initially it's visible
         * and will indicate that it received the focus.
         */
		base() : Visible(true), Selhilite(true) 
        {
            Look = new decoration();
        }
        
        /**
         * Cleanup.
         */
        virtual ~base ()
        {
            delete Look;
        }

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
		virtual bool keyup(input::keyboard_event & k) { return false; }
		
        /**
         * Called when a key has been pressed by the user.
         * @param k the keyboard event.
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool keydown(input::keyboard_event & k) { return false; }
		
        /**
         * Called when text has been input by the user.
         * @param k the keyboard event.
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool input(input::keyboard_event & k) { return false; }
        //@}
#if 0		
		/*
		 * called when a mouseevent happens, returns whether it was used
		 */
		virtual bool mouseup(SDL_MouseButtonEvent & m) {return false; }
		virtual bool mousedown(SDL_MouseButtonEvent & m) {return false; }
		virtual bool mousemove(SDL_MouseMotionEvent & m) {return false; }
#endif
		/**
         * Called when object is offered the keyboard focus. Returns true
		 * to accept it.
         * @return true to accept focus, false to decline.
		 */
		virtual bool focus() { return false; }
		
        /**
         * Notification that the object will no longer recieve keyboard 
         * events
         */
		virtual void unfocus() {}

		/**
         * Change the size of the widget 
         * @param width the new width
         * @param height the new height
         */
		virtual void set_size(const u_int16 & width, const u_int16 & height) 
        {  
            set_length (width);
            set_height (height);
            Look->set_size (width, height);
        }

        /**
         * Return whether the widget should indicate that is has focus.
         * @return true if it indicates focus, false otherwise.
         */
		bool highlightEnabled() const 
        { 
            return Selhilite; 
        }

#ifndef SWIG
        GET_TYPE_NAME_ABSTRACT(gui::base);
#endif
        
    protected:
        /// whether widget is visible
		bool Visible;
        /// whether widget indicates focus
		bool Selhilite;
        /// graphical representation of the widget
        decoration *Look;
	};
};

#endif//GUI_BASE_H
