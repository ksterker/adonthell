/*
   $Id: base.h,v 1.10 2004/12/21 22:03:17 jol Exp $

   Copyright (C) 1999/2000/2001/2002   Alexandre Courbot <alexandrecourbot@linuxgames.com>
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

#ifndef GUI_BASE_H_
#define GUI_BASE_H_

#include "gfx/surface.h"
#include "gfx/screen.h"
#include "gfx/drawing_area.h"
#include "input/input.h"

namespace gui {
  
  class container;
  
  class base : protected gfx::drawing_area
    {
    
    public:
    
      /**
       * Default Constructor. initialize some data
       */
      base ();

      /**
       * Return the X relative position of an object.
       * @return the X relative position
       */
      s_int16 get_x () const { return m_x; }
      
      /**
       * Return the Y relative position of an object.
       * @return the Y relative position
       */
      s_int16 get_y () const { return m_y; }
      
      /**
       * Return the Pad X (by default there is no pad)
       * @return the Pad X
       */
      s_int16 get_pad_x () const { return m_padx; }
      
      /**
       * Return the Pad Y (by default there is no pad)
       * @return the Pad Y
       */
      s_int16 get_pad_y () const { return m_pady; }
      
      /**
       * Return the X absolute (Vodka?) position of this object.
       * @return the X absolute position.
       */
      s_int16 get_real_x () const
	{ return gfx::drawing_area::x (); }
      
      /**
       * Return the Y absolute (Vodka?) position of this object.
       * @return the Y absolute position.
       */
      s_int16 get_real_y () const
	{ return gfx::drawing_area::y (); }
      
      /**
       * Return the X absolute (Vodka?) position of this object.
       * @return the X absolute position.
       */
      u_int16 get_height () const
	{ return gfx::drawing_area::height (); }
      
      u_int16 get_length () const
	{ return gfx::drawing_area::length (); }
      
      /**
       * Define the location of the window
       * - the update_position method is called.
       * @param nx: The new X position.
       * @param ny: The new Y position.
       */
      virtual void set_location (s_int16 nx, s_int16 ny);

      
      /**
       * Some times we must make an update (by example: for animation)
       * I think we can plug an Event engine to be sure do not call this method every time
       */
      virtual bool update ();
      
      /**
       * If the Object is visible, draw the object.
       *  - assign this object to the drawing area passed as parameter,
       *  - call draw contents,
       *  - detach the drawing area
       * @param sf: The surface where we can draw. If the surface is null then use the screen surface
       * @param da: the drawing area border.
       */
      bool draw (gfx::surface * sf = gfx::screen::get_surface(), gfx::drawing_area * da = NULL);
      

      /**
       * Draw the contents of this object even if the object is not visible.
       * @param sf: The surface must not be null
       * 
       */
      virtual bool draw_contents (gfx::surface * sf);

      /**
       * Assign a drawing area for this object
       */
      void assign_area (gfx::drawing_area * da)
	{ this->assign_drawing_area (da); }
      
      /**
       * Detach the drawing area
       */
      void detach_area ()
	{ this->detach_drawing_area (); }

      /**
       * Update the position
       */
      virtual void update_position ();
      
      /**
       * Update the Size
       */
      virtual void update_size ();

      /**
       * Define the size of the component
       * - the update_size method is called.
       */
      void set_size (u_int16 nl, u_int16 nh);
      
      /**
       * Define if this object is visible
       * @param b: if true the object become visible
       */
      void set_visible (const bool b) { m_visible = b; }
      
      /**
       * Return true if the object is visible
       * @return true if the object is visible.
       */
      bool is_visible () const { return m_visible;}

      /**
       * Enable this object. An enabled object can be editable
       */
      void set_enabled (const bool b) { m_enabled = b; }

      /**
       * Return true is the object is enabled. By default it's enabled
       * @return true if the project is enabled
       */
      bool is_enabled () const { return m_enabled; }

      /**
       * Select this object.
       */
      void set_selected (const bool b) { m_selected = b; }

      /**
       * Return true is the object is selected. By default it's not selected
       * @return true if the project is selected
       */
      bool is_selected () const { return m_selected; }

      /**
       * Specify that this object is selectable
       */
      void set_selectable (const bool b) { m_selectable = b; }

      /**
       * Return true is the object is selectable. By default it's selectable
       * @return true if the project is selectable
       */
      bool is_selectable () const { return m_selectable; }

      /**
       * Set the focus on this object.
       */
      void set_focus (const bool b) { m_focus = b; }

      /**
       * Return true is the object has the focus.
       * @return true if the project has the focus
       */
      bool is_focus () const { return m_focus; }

      /**
       * Set the parent of this object. 
       * - update position.
       */
      void set_parent (container * parent);
      
      /**
       * Get the parent of this object
       * @return the Parent
       */
      container * get_parent () { return m_parent; }

      /**
       * Return the drawing area of this object
       */
      gfx::drawing_area * get_drawing_area () { return (gfx::drawing_area*) this; }

      /**
       * Return the Parent drawing Area. If no parent return null
       */
      gfx::drawing_area * get_parent_drawing_area ();
      
      /**
       * Align this object in vertical
       */
      void set_vertical_align ( u_int8 align);

      /**
       * Align this object in horizontal
       */
      void set_horizontal_align ( u_int8 align);

      /**
       * Set Listener. By default a basic object have no listener
       * If there was already a listener remove it 
       */
      void set_listener (::input::listener * list, u_int8 device = 0);
       
      /**
       * Return the listener of this composant
       */
      ::input::listener * get_listener () const;

      /**
       * Define the Input methods
       */
      virtual bool on_mouse_event (input::mouse_event * evt);

      virtual bool on_keyboard_event (input::keyboard_event * evt);
      
      virtual bool on_joystick_event (input::joystick_event * evt);
      
      virtual bool on_control_event (input::control_event * evt);
      
      /**
       * Free memory
       */
      virtual ~base ();
      
      /* Constant value */
      static const u_int8 ALIGN_NONE = 0;
      static const u_int8 ALIGN_LEFT = 1;
      static const u_int8 ALIGN_RIGHT = 2;
      static const u_int8 ALIGN_CENTER = 3;
      static const u_int8 ALIGN_TOP = 4;
      static const u_int8 ALIGN_BOTTOM = 5;

      /* Constant Value for select device, maybe move this stuff to input namespace*/
      static const u_int8 NO_DEVICE = 0;
      static const u_int8 KEYBOARD_DEVICE = 1;
      static const u_int8 MOUSE_DEVICE = 2;
      static const u_int8 JOYSTICK_DEVICE = 4;
      static const u_int8 CONTROL_DEVICE = 8;

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::base)
#endif // SWIG

    protected:
      
      s_int16 m_x; //virtual position
      
      s_int16 m_y; //virutal position
      
      s_int16 m_padx; // x padding
      
      s_int16 m_pady; // y padding

      u_int8 m_vertical_align; // vertical alignment
      
      u_int8 m_horizontal_align; // horizontal alignment
      
      bool m_visible; // if the object is visible
      
      bool m_focus; // if the object has focus
      
      bool m_selected; // if the object is selected

      bool m_selectable; // if an object can be selected

      bool m_enabled; // if the object is enable
      
      container * m_parent; // a pointer to his parent
      
      ::input::listener * m_listener;

      u_int32 m_id;

      static u_int32 gui_objects;
      
    };
}

#endif
