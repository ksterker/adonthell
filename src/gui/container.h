/*
   $Id: container.h,v 1.4 2003/11/22 09:37:13 ksterker Exp $

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
