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
 * @file   gui/layout.h
 * @author Rian Shelley
 * @brief  Defines the layout class.
 */

#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include "widget.h"

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
        
		/**
		 * Destructor.
		 */
		virtual ~layoutchild() {}

        /// the widget
		widget* Child;
        /// the widgets size and position
        gfx::drawing_area Pos;
	};
#endif

    /**
     * Base class for containers that group multiple
     * widgets in a specific layout. This container allows
     * absolute positioning of its children.
     */
	class layout : public widget
	{
	public:

		typedef enum
		{
			NONE 		= 0,
			GROW_X 		= 1,
			GROW_Y 		= 2,
			GROW_BOTH 	= GROW_X | GROW_Y
		} resize_mode;

        /**
         * Create a new, empty layout with the given size.
         * @param l layout length
         * @param h layout height
         */
		layout(const u_int16 & l, const u_int16 & h) : widget(l, h), Selected(0), Focused(false)
        {
            Selhilite = false;
            Listener = NULL;
            ResizeMode = GROW_BOTH;
        }

        /**
         * Create layout widget from the given data file.
         * Size will be set to the background size.
         * @param style filename of widget decoration.
         */
		layout (const std::string & style)
		: widget(style), Selected(0), Focused(false)
		{
            Selhilite = false;
            Listener = NULL;
            ResizeMode = GROW_BOTH;
		}

		/**
		 * Delete layout.
		 */
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
		void add_child (gui::widget & c, const s_int16 & x, const s_int16 & y); 
        
        /** 
         * Remove UI element from the layout.
         * @param c previously added child.
         */
		void remove_child (gui::widget & c);

		/**
		 * Get the child widget at the given index.
		 * @param index the index of the child.
		 * @return child at given index.
		 */
		const gui::widget& get_child (const u_int32 & index) const
		{
			return *Children[index].Child;
		}

		/**
		 * Get the area covered by the given child.
		 * @return position and area of the given child.
		 */
		const gfx::drawing_area& get_location (const gui::widget & c) const;

		/**
		 * Get the number of children in this container.
		 * @return the number of children in the container.
		 */
		u_int32 num_children () const { return Children.size(); }
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
         * Set an input listener so this widget can receive keyboard input. Any
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

        /**
         * Callback executed whenever a button on the joystick has been pushed.
         * @param evt the button press event
         * @return whether the event was consumed or not.
         */
        bool on_joystick_event (input::joystick_event *evt);
        //@}

        /**
         * Visibility.
         */
        //@{
		bool visible () const { return Visible; }
        //@}

		/**
		 * @name Size Handling
		 */
		//@{
		/**
		 * Set whether the layout can shrink or expand to
		 * accommodate for its children.
		 * @param mode the growth mode.
		 */
		void set_auto_grow (const gui::layout::resize_mode & mode)
		{
			ResizeMode = mode;
		}

		/**
		 * Resize the layout to accommodate for its children.
		 * May grow or shrink the layout so that all children fit.
		 * @param mode the growth mode.
		 */
		virtual void resize (const gui::layout::resize_mode & mode = GROW_BOTH);
		//@}

        /**
         * Focus handling.
         */
        //@{
        /**
         * Called when the container receives the focus.
         * @return true when a child accepts the focus.
         */
		virtual bool focus();

        /**
         * Called when the container loses focus.
         */
		virtual void unfocus()
        {
            if (Children.size()) Children[Selected].Child->unfocus();
            Focused = false;
        }

		/**
		 * Index of UI element that currently has the focus
		 * in this container.
		 * @return index of selected element.
		 */
		u_int32 current () const { return Selected; }
        //@}

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::layout);
#endif

    protected:
        typedef enum
        {
            NEXT = 0,
            PREV = 1
        } select_direction;

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
        
        /// widgets kept in the container
        std::vector<layoutchild> Children;
		/// child which currently is selected
        u_int32 Selected;
        /// whether this container has the focus
		bool Focused;
        /// the input handler
        input::listener *Listener;
        /// whether to resize the layout when children are added
        resize_mode ResizeMode;

		/**
         * Called when the user pressed the move right key.
         * @return true if a child became the newly focused element.
         */
        virtual bool moveright();
		/**
         * Called when the user pressed the move left key.
         * @return true if a child became the newly focused element.
         */
		virtual bool moveleft();
		/**
         * Called when the user pressed the move up key.
         * @return true if a child became the newly focused element.
         */
		virtual bool moveup();
		/**
         * Called when the user pressed the move down key.
         * @return true if a child became the newly focused element.
         */
		virtual bool movedown();

        /**
         * Select the next or previous child.
         * @param direction NEXT or PREV
         * @return true if a child became the newly focused element.
         */
        virtual bool select_child(gui::layout::select_direction direction);

		// void dofade(int x, int y, const SDL_Surface* s);
		// bool mousestate[3];
	};
}

#endif//GUI_BASE_H
