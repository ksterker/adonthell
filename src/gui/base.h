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
      
      virtual void setLocation (s_int16 nx, s_int16 ny);

      virtual bool update ();

      bool draw (); //gfx::drawing_area * da);
      
      virtual bool drawContents ();

      void assignArea (gfx::drawing_area * da)
	{ this->assign_drawing_area (da); }
      
      void detachArea ()
	{ this->detach_drawing_area (); }

      virtual void updatePosition ();
      
      virtual void updateSize ();

      void setSize (u_int16 nl, u_int16 nh);
      
      void setVisible (const bool b)
	{ m_visible = b; }

      bool isVisible () const
	{ return m_visible;}

      void setEnable (const bool b)
	{ m_enable = b; }

      bool isEnable () const
	{ return m_enable; }

      void setParent (container * parent);

      container * getParent ()
	{ return m_parent; }

      gfx::drawing_area * getDrawingArea ()
	{ return (gfx::drawing_area*) this; }

      gfx::drawing_area * getParentDrawingArea ();
	

      void setVerticalAlign ( u_int8 align);
      
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
