#ifndef GUI_PARENT_H_
#define GUI_PARENT_H_

#include<list>

#include "gui/base.h"


namespace gui {

  class container : public base
    {
    public:
      
      virtual void addChild (base * m);

      virtual void removeChild (base * m);
      
      virtual void updatePosition ();
      
      virtual void updateLayout ();

      virtual bool drawContents ();

      void destroyAll ();

      virtual ~container ();
    protected:
      
      typedef std::list <base * > ListChild;
      ListChild m_childs;
      
    };
}

#endif
