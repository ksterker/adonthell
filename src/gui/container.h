/*
   $Id: container.h,v 1.9 2004/05/13 06:44:00 ksterker Exp $

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

      /**
       * Add an element in this container
       */
      virtual void addChild (base * m);

      /**
       * Remove an element
       */
      virtual void removeChild (base * m);
      
      /**
       * Update the position of this object an all object inside its
       */
      virtual void updatePosition ();
      
      /**
       * Make an layout update
       */
      void updateLayout ();

      /**
       * Draw the contents of this object
       */
      virtual bool drawContents (gfx::surface * sf);
      
      /**
       * Define the space between 2 childs
       */
      void setSpaceChild (s_int16 space);
      
      /**
       * Get The Space between each child
       */
      s_int16 getSpaceChild () { return m_space_child; }
      
      /**
       * Get space between Child and border
       */
      s_int16 getSpaceBorder () { return m_space_border; }

      /**
       * Define the space between a child and the border
       */
      void setSpaceBorder (s_int16 space);

      /**
       * Define the layout used by this container
       */
      void setLayout (layout * l);

      /**
       * Delete from memory all child
       */
      void destroyAll ();

      /**
       * Get all childs
       */
      ListChild & getChilds () { return m_childs; }

      /**
       * Define the policy used to extend the container
       */
      void setExtendPolicy (u_int8 extend);

      /**
       * Destructor: free all memory
       */ 
      virtual ~container ();

      /**
       * DEFINE POLICY
       */
      static const u_int8 POLICY_NORMAL = 0;
      static const u_int8 POLICY_EXPAND_CHILD = 1;
      static const u_int8 POLICY_EXPAND_CONTAINER = 2;
      static const u_int8 POLICY_STRETCH_CHILD = 3;

#ifndef SWIG
        GET_TYPE_NAME_VIRTUAL(gui::container)
#endif // SWIG

    protected:

      layout * m_layout; // the layout used
      
      ListChild m_childs; // the list of child
      
      s_int16 m_space_child; // space between each object

      s_int16 m_space_border; // space between child and order of the container
    
      u_int8 m_extend; // the policy used to extend the container
    };
}

#endif
