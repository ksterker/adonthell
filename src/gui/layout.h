/*
   $Id: layout.h,v 1.5 2008/03/08 20:07:53 ksterker Exp $

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

#ifndef GUI_LAYOUT_H_
#define GUI_LAYOUT_H_

namespace gui
{
  class container;
  
  class layout
    {
    public:
      layout ();
	  virtual ~layout() { }
      
      virtual bool update ();
      
      void set_container (container * ct);

      u_int16 get_max_length () const 
	{ return m_length; }

      u_int16 get_max_height () const 
	{return m_height; }
      
    protected:
      
      // define the space needed for this layout
      // the space contains spaceborder and spacechild information
      // from the container. m_length is the max size, m_height 
      // the max height.
      u_int16 m_length; 
      u_int16 m_height;

      container * m_container;
    };
}

#endif
