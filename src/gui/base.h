/*
   $Id: base.h,v 1.5 2004/01/06 22:39:56 jol Exp $

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

#include "gfx/drawing_area.h"

namespace gui {
  
  class container;
  
  class base : protected gfx::drawing_area
    {
    
    public:
    
      base ();
      
      s_int16 getX () const 
	{ return m_x; }
      
      s_int16 getY () const
	{ return m_y; }
      
      s_int16 getPadX () const 
	{ return m_padx; }
      
      s_int16 getPadY () const
	{ return m_pady; }
      
      s_int16 getRealX () const
	{ return gfx::drawing_area::x (); }
      
      s_int16 getRealY () const
	{ return gfx::drawing_area::y (); }
      
      u_int16 getHeight () const
	{ return gfx::drawing_area::height (); }
      
      u_int16 getLength () const
	{ return gfx::drawing_area::length (); }
      
      /**
       * Define the location of the window
       */
      virtual void setLocation (s_int16 nx, s_int16 ny);

      /**
       * Some times we must make an update (by example: for animation)
       */
      virtual bool update ();

      /**
       * Draw this object
       */
      bool draw (); //gfx::drawing_area * da);
      

      /**
       * Draw the contents of this object
       */
      virtual bool drawContents ();

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
      
      void setVisible (const bool b)
	{ m_visible = b; }

      bool isVisible () const
	{ return m_visible;}

      /**
       * Enable this object. An enabled object can be editable
       */
      void setEnable (const bool b)
	{ m_enable = b; }

      bool isEnable () const
	{ return m_enable; }

      void setParent (container * parent);

      /**
       * Get the parent of this object
       */
      container * getParent ()
	{ return m_parent; }

      /**
       * Return the drawing area of this object
       */
      gfx::drawing_area * getDrawingArea ()
	{ return (gfx::drawing_area*) this; }

      gfx::drawing_area * getParentDrawingArea ();
	
      /**
       * Align this object in vertical
       */
      void setVerticalAlign ( u_int8 align);

      /**
       * Align this object in horizontal
       */
      void setHorizontalAlign ( u_int8 align);

      virtual ~base ();
      
      /* Constant value */
      static const u_int8 ALIGN_NONE = 0;
      static const u_int8 ALIGN_LEFT = 1;
      static const u_int8 ALIGN_RIGHT = 2;
      static const u_int8 ALIGN_CENTER = 3;
      static const u_int8 ALIGN_TOP = 4;
      static const u_int8 ALIGN_BOTTOM = 5;

    protected:
      
      s_int16 m_x; //virtual position
      
      s_int16 m_y; //virutal position
      
      s_int16 m_padx; // x padding
      
      s_int16 m_pady; // y padding

      u_int8 m_vertical_align; // vertical alignment
      
      u_int8 m_horizontal_align; // horizontal alignment
      
      bool m_visible; //if the object is visible
      
      // bool m_focus; // if the object has focus
      
      bool m_enable; // if the object is enable
      
      container * m_parent; // a pointer to his parent
      
    private:
      
    };
}

#ifndef SWIG
PYTHON_AS_CALLBACK_ARGUMENT(gui::base)
#endif // SWIG

#endif
