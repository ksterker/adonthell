/*
   $Id: base.h,v 1.8 2004/04/29 08:07:49 ksterker Exp $

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
      s_int16 getX () const { return m_x; }
      
      /**
       * Return the Y relative position of an object.
       * @return the Y relative position
       */
      s_int16 getY () const { return m_y; }
      
      /**
       * Return the Pad X (by default there is no pad)
       * @return the Pad X
       */
      s_int16 getPadX () const { return m_padx; }
      
      /**
       * Return the Pad Y (by default there is no pad)
       * @return the Pad Y
       */
      s_int16 getPadY () const { return m_pady; }
      
      /**
       * Return the X absolute (Vodka?) position of this object.
       * @return the X absolute position.
       */
      s_int16 getRealX () const
	{ return gfx::drawing_area::x (); }
      
      /**
       * Return the Y absolute (Vodka?) position of this object.
       * @return the Y absolute position.
       */
      s_int16 getRealY () const
	{ return gfx::drawing_area::y (); }
      
      /**
       * Return the X absolute (Vodka?) position of this object.
       * @return the X absolute position.
       */
      u_int16 getHeight () const
	{ return gfx::drawing_area::height (); }
      
      u_int16 getLength () const
	{ return gfx::drawing_area::length (); }
      
      /**
       * Define the location of the window
       * @param nx: The new X position.
       * @param ny: The new Y position.
       */
      virtual void setLocation (s_int16 nx, s_int16 ny);

      
      /**
       * Some times we must make an update (by example: for animation)
       * I think we can plug an Event engine to be sure do not call this method every time
       */
      virtual bool update ();
      
      /**
       * If the Object is visible, draw the object.It's calling drawContents.
       * @param sf: The surface where we can draw. If the surface is null then use the screen surface
       */
      bool draw (gfx::surface * sf = NULL, gfx::drawing_area * da = NULL);
      

      /**
       * Draw the contents of this object even if the object is not visible.
       * @param sf: The surface must not be null
       * 
       */
      virtual bool drawContents (gfx::surface * sf);

      /**
       * Assign a drawing area for this object
       */
      void assignArea (gfx::drawing_area * da)
	{ this->assign_drawing_area (da); }
      
      /**
       * Detach the drawing area
       */
      void detachArea ()
	{ this->detach_drawing_area (); }

      /**
       * Update the position
       */
      virtual void updatePosition ();
      
      /**
       * Update the Size
       */
      virtual void updateSize ();

      void setSize (u_int16 nl, u_int16 nh);
      
      /**
       * Define if this object is visible
       * @param b: if true the object become visible
       */
      void setVisible (const bool b) { m_visible = b; }
      
      /**
       * Return true if the object is visible
       * @return true if the object is visible.
       */
      bool isVisible () const { return m_visible;}

      /**
       * Enable this object. An enabled object can be editable
       */
      void setEnabled (const bool b) { m_enabled = b; }

      /**
       * Return true is the object is enabled. By default it's enabled
       * @return true if the project is enabled
       */
      bool isEnabled () const { return m_enabled; }

      /**
       * Select this object.
       */
      void setSelected (const bool b) { m_selected = b; }

      /**
       * Return true is the object is selected. By default it's not selected
       * @return true if the project is selected
       */
      bool isSelected () const { return m_selected; }

      /**
       * Specify that this object is selectable
       */
      void setSelectable (const bool b) { m_selectable = b; }

      /**
       * Return true is the object is selectable. By default it's selectable
       * @return true if the project is selectable
       */
      bool isSelectable () const { return m_selectable; }

      /**
       * Set the focus on this object.
       */
      void setFocus (const bool b) { m_focus = b; }

      /**
       * Return true is the object has the focus.
       * @return true if the project has the focus
       */
      bool isFocus () const { return m_focus; }

      /**
       * Set the parent of this object. 
       * It's calling update Position.
       * 
       */
      void setParent (container * parent);
      
      /**
       * Get the parent of this object
       * @return the Parent
       */
      container * getParent () { return m_parent; }

      /**
       * Return the drawing area of this object
       */
      gfx::drawing_area * getDrawingArea () { return (gfx::drawing_area*) this; }

      /**
       * Return the Parent drawing Area. If no parent return null
       */
      gfx::drawing_area * getParentDrawingArea ();
      
      /**
       * Align this object in vertical
       */
      void setVerticalAlign ( u_int8 align);

      /**
       * Align this object in horizontal
       */
      void setHorizontalAlign ( u_int8 align);

      /**
       * Set Listener. By default a basic object have no listener
       * If there was already a listener remove it 
       */
      void setListener (::input::listener * list, u_int8 device = 0);
       
      /**
       * Return the listener of this composant
       */
      ::input::listener * getListener () const;

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
