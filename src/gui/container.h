#ifndef GUI_PARENT_H_
#define GUI_PARENT_H_

#include<list>

#include "gui/base.h"
#include "gui/layout.h"

namespace gui {
  
  typedef std::list <base * > ListChild;
  
  class container : public base
    {
    public:
      container ();

      virtual void addChild (base * m);
      
      virtual void removeChild (base * m);
      
      virtual void updatePosition ();
      
      void updateLayout ();

      virtual bool drawContents ();

      void setSpaceChild (s_int16 space);
      
      s_int16 getSpaceChild ()
	{ return m_space_child; }
      
      s_int16 getSpaceBorder ()
	{ return m_space_border; }

      void setSpaceBorder (s_int16 space);

      void setLayout (layout * l);

      void destroyAll ();

      ListChild & getChilds ()
	{ return m_childs; }

      virtual ~container ();
    protected:

      layout * m_layout;
      
      ListChild m_childs;
      
      s_int16 m_space_child; // space between each object

      s_int16 m_space_border; // space between child and order of the container
    };
}

#ifndef SWIG
PYTHON_AS_CALLBACK_ARGUMENT(gui::container)
#endif // SWIG

#endif
