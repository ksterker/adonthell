/*
   $Id: container.cc,v 1.4 2003/11/22 09:37:13 ksterker Exp $

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

using namespace gui;


container::container () : base ()
{
  m_layout = NULL;
  m_space_child = m_space_border = 5;
  
}

void container::addChild (base * m)
{
  m_childs.push_back (m);
  
  m->setParent (this);
  
  m->updatePosition ();

  // WARNING: I think update layout must be called manually
  // because if there is a lot's of childs it can take a long time ...
  updateLayout ();
}

void container::removeChild (base * m)
{
  m_childs.remove (m);
  
  m->setParent (NULL);
  
  m->updatePosition ();

  updateLayout ();
}

void container::updatePosition ()
{
  base::updatePosition (); // update his position

  for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
    (*i)->updatePosition ();
}

void container::updateLayout ()
{
  // by default no layout ...
  // we can imagine a solution with a pluggable system
  // to set a specific manage
  if (m_layout) m_layout->update ();
}

void container::destroyAll ()
{
  for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
    delete *i;
  m_childs.clear ();
}

bool container::drawContents ()
{
  if (base::drawContents ())
    {
      for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
	{
	  (*i)->assignArea (this);
	  (*i)->draw ();
	  (*i)->detachArea ();
	}
    }
  return false;
}

void container::setSpaceChild (s_int16 space)
{
  m_space_child = space;
  updateLayout ();
}

void container::setSpaceBorder (s_int16 space)
{
  m_space_border = space;
  updateLayout ();
}

container::~container ()
{
  destroyAll ();
  if (m_parent) m_parent->removeChild (this);
  if (m_layout) delete m_layout;
}

void container::setLayout (layout * l)
{
  
  if (m_layout) delete m_layout;
  m_layout = l;
  if (m_layout) m_layout->setContainer (this);
  updateLayout ();
}
