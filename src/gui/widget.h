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
 * @file   gui/widget.h
 * @author Rian Shelley
 * @brief  Defines the widget class.
 */

#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include "gfx/gfx.h"
#include "gfx/drawable.h"
#include "gfx/surface_cacher.h"

#include "input/input.h"

#include "gui/decoration.h"

namespace gui
{
	/** 
     * The base class for displayable elements
     */
	class widget : public gfx::drawable
	{
	public:
        /**
         * Create widget of given size.
         * @param width the initial width.
         * @param height the inital height.
         */
		widget(const u_int16 & width, const u_int16 & height)
        : Visible(true), Selhilite(true)
        {
            Look = new decoration();
            set_size (width, height);
        }
        
        /**
         * Create widget from the given data file.
         * Size will be set to the background size.
         * @param name filename of widget decoration.
         */
		widget (const std::string & name)
        : Visible(true), Selhilite(true)
        {
            u_int16 w = 0, h = 0;

            Look = new decoration();
            if (Look->init (name))
            {
                Look->set_state ();
                Look->get_size (w, h);
            }
            
            set_length (w);
            set_height (h);
        }
        
        /**
         * Destroy widget and its input listener.
         */
		virtual ~widget()
        {
            delete Look;
        }
        
        /** 
         * Draw the object on the %screen.
         * 
         * @param x X position where to draw.
         * @param y Y position where to draw.
         * @param da optional drawing_area to use during the drawing operation.
         * @param target pointer to the surface where to draw the drawable. If NULL,
         *               draw on the screen.
         */		
        virtual void draw (const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da = NULL, gfx::surface * target = NULL) const;
        
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
         * @name Focus Handling
         */
        //@{
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
         * Make the widget indicate that is has focus. Whether this
         * actually happens also depends on the value of #Selhilite.
         * If this is set to false, the widget will never indicate
         * that it has the focus.
         *
         * @param enable true to indicates focus, false otherwise.
         */
		void enable_focus (const bool & enable) 
        { 
            Look->set_focused (Selhilite && enable);
        }
        //@}
        
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
        
#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::widget);
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

#endif//GUI_WIDGET_H

