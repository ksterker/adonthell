/*
   $Id: listlayout.cc,v 1.6 2004/12/21 22:03:17 jol Exp $

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

#include <iostream>
#include <cassert>
#include "gui/container.h"
#include "gui/listlayout.h"

using namespace gui;


listlayout::listlayout (): layout ()
{
  
}

bool listlayout::update ()
{
  assert (m_container != NULL);

  if (m_container) // be sure that there is a container
    {
      // define the initial value for x
      s_int16 nx = m_container->get_pad_x () + m_container->get_space_border (), 
	ny =  m_container->get_pad_y () + m_container->get_space_border ();
      
      m_height = m_length = 0;

      for (ListChild::iterator i = m_container->get_childs ().begin (); 
	   i != m_container->get_childs ().end (); ++i)
	{
	  (*i)->set_location (nx, ny);
	  ny += (*i)->get_height () + m_container->get_space_child ();
	  if ((*i)->get_length () > m_length) m_length = (*i)->get_length ();
	}
      m_height = ny - m_container->get_space_child () + m_container->get_space_border ();
      m_length += m_container->get_pad_x () + (m_container->get_space_border () << 1);
      return true;
    }
  return false;
}
