/*
   $Id: listlayout.cc,v 1.3 2003/11/22 09:37:13 ksterker Exp $

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
#include "gui/container.h"
#include "gui/listlayout.h"

using namespace gui;



listlayout::listlayout (): layout ()
{

}

bool listlayout::update ()
{
  if (m_container)
    {
      s_int16 nx = m_container->getPadX () + m_container->getSpaceBorder (), 
	ny =  m_container->getPadY () + m_container->getSpaceBorder ();
      for (ListChild::iterator i = m_container->getChilds ().begin (); 
	   i != m_container->getChilds ().end (); ++i)
	{
	  (*i)->setLocation (nx, ny);
	  ny += (*i)->getHeight () + m_container->getSpaceChild ();
	}
      return true;
    }
  return false;
}
