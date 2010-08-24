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

#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include <vector>

#include "gui/widget.h"
#include "input/input.h"
#include "input/listener.h"

namespace gui
{
#ifndef SWIG
    /**
     * Position of widget in the layout.
     */
	class layoutchild
	{
	public:
        /**
         * Create a new position structure.
         * @param b the widget to position.
         * @param d the widgets coordinates.
         */
		layoutchild (gui::widget *b, gfx::drawing_area & d) : Child(b), Pos(d) { }
        
        /// the widget
		widget* Child;
        /// the widgets size and position
        gfx::drawing_area Pos;
	};
#endif

    /**
     * Base class for containers that group multiple
     * widgets in a specifc layout. This container allows
     * absolute positioning of its children.
     */
	class layout : public widget
	{
	public:
        /**
         * Create a new, empty layout.
         */
		layout() : widget(0, 0), Selected(0), focused(false)
        {
            Selhilite = false;
            Listener = NULL;
        }
        
        /**
         * Create a new, empty layout with the given size.
         * @param l layout length
         * @param h layout height
         */
		layout(const u_int16 & l, const u_int16 & h) : widget(l, h), Selected(0), focused(false)
        {
            Selhilite = false;
            Listener = NULL;
        }

        virtual ~layout()
        {
            delete Listener;
        }
        
        /**
         * @name Container elements.
         */
        //@{
        /**
         * Add given UI element to the layout.
         * @param c the child to add.
         * @param x the x-coordinate to place child at.
         * @param y the y-coordinate to place child at.
         */
		void addchild (gui::widget & c, const s_int16 & x, const s_int16 & y); 
        
        /** 
         * Remove UI element from the layout.
         * @param c previously added child.
         */
		void removechild (gui::widget & c);
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
        virtual void draw(const s_int16 & x, const s_int16 & y, const gfx::drawing_area * da = NULL, gfx::surface * target = NULL) const;
		
        /**
         * @name Input focus handling
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
        
        //pending mouse support
//		virtual bool mouseup(SDL_MouseButtonEvent & m);
//		virtual bool mousedown(SDL_MouseButtonEvent & m);
//		virtual bool mousemove(SDL_MouseMotionEvent & m) { return false; }

        /**
         * Focus handling.
         */
        //@{
        /**
         * Called when the container recieves the focus.
         * @return true when a child accepts the focus.
         */
		virtual bool focus();
        
        /**
         * Called when the container loses focus.
         */
		virtual void unfocus() 
        {
            if (Children.size()) Children[Selected].Child->unfocus(); 
            focused = false;
        }
        //@}

        /**
         * Visibility.
         */
        //@{
		bool visible () const { return Visible; }
        //@}
        
#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::layout);
#endif
    protected:
        /// widgets kept in the container
        std::vector<layoutchild> Children;
		/// child which currently is selected
        u_int32 Selected;
        /// whether this container has the focus
		bool focused;
        /// the input handler
        input::listener *Listener;

		/**
         * Called when the user pressed the move right key.
         * @return true if a child became the newly focussed element.
         */
        virtual bool moveright();
		/**
         * Called when the user pressed the move left key.
         * @return true if a child became the newly focussed element.
         */
		virtual bool moveleft();
		/**
         * Called when the user pressed the move up key.
         * @return true if a child became the newly focussed element.
         */
		virtual bool moveup();
		/**
         * Called when the user pressed the move down key.
         * @return true if a child became the newly focussed element.
         */
		virtual bool movedown();
        
		// void dofade(int x, int y, const SDL_Surface* s);
		// bool mousestate[3];
	};
};

#endif//GUI_BASE_H
