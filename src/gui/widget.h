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
#include "gfx/surface_cacher.h"
#include "input/listener.h"
#include "gui/base.h"

namespace gui
{
	/** 
     * The base class for displayable elements
     */
	class widget : public base
	{
	public:
        /**
         * Create widget of given size.
         * @param width the initial width.
         * @param height the inital height.
         */
		widget(const u_int16 & width, const u_int16 & height) 
        : Listener(NULL) 
        {
            set_size (width, height);
        }
        
        /**
         * Create widget from the given data file.
         * Size will be set to the background size.
         * @param name filename of widget decoration.
         */
		widget (const std::string & name) 
        {
            u_int16 w = 0, h = 0;
            if (Look->init (name))
            {
                Look->set_state ();
                Look->get_size (w, h);
            }
            
            Listener = NULL;
            set_length (w);
            set_height (h);
        }
        
		virtual ~widget()
        {
            delete Listener;
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
         * @name Input handling
         */
        //@{
        /**
         * Set an input listener so this widget can recieved keyboard input. Any
         * previously set input listeners are deleted.
         * @param lstnr the input listener to set.
         */
        void set_listener (input::listener *lstnr);

        /**
         * Get the input listener associated to that widget, if any.
         * @return this widgets input listener.
         */
        input::listener * get_listener () const;
        
        /**
         * Callback executed whenever a key on the keyboard has been pushed.
         * @param evt the key press event
         * @return whether the event was consumed or not.
         */
        bool on_keyboard_event (input::keyboard_event * evt);
        //@}

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::widget);
#endif
        
    protected:
        /// the input handler
        input::listener *Listener;
	};
};

#endif//GUI_WIDGET_H

