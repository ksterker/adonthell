/*
   $Id: container.cc,v 1.9 2004/02/07 19:45:15 jol Exp $

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

using namespace gui;

container::container () : base ()
{
  m_layout = NULL;
  m_space_child = m_space_border = 5;
  setExtendPolicy (POLICY_NORMAL);
}

void container::addChild (base * m)
{
  m_childs.push_back (m);
  
  m->setParent (this);
  
  m->updatePosition ();

  // WARNING: I think update layout must be called manually
  // because if there is a lot's of childs it can take a long time ...
  updateLayout ();

#ifdef DEBUG
  std::cout << "Nd childs: " << (int) m_childs.size () << std::endl; 
#endif DEBUG
}

void container::removeChild (base * m)
{
  
  m_childs.remove (m);
  
  m->setParent (NULL);
  
  m->updatePosition ();

  updateLayout ();
#ifdef DEBUG
  std::cout << "Nd childs: " << (int) m_childs.size () << std::endl; 
#endif
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
  if (m_layout) {
    m_layout->update ();
    
    // check the policy used
    u_int16 l = getLength (), h = getHeight ();
    switch (m_extend) {
    case POLICY_EXPAND_CONTAINER:
      if (m_layout->getMaxLength () > l) l = m_layout->getMaxLength ();
      if (m_layout->getMaxHeight () > h) h = m_layout->getMaxHeight ();
      setSize (l, h);
      updateLayout ();
      break;
    
    case POLICY_EXPAND_CHILD:
    case POLICY_STRETCH_CHILD:
      std::cerr << "Not implemented : " << (u_int8) m_extend << " " << __FILE__ << " " << __LINE__ << std::endl;
      assert (false);
    }
  }
}

void container::destroyAll ()
{
  for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
    delete *i;
  m_childs.clear ();
}

bool container::drawContents (gfx::surface * sf) {
  if (base::drawContents (sf)) {
    for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
      (*i)->draw (sf, this);
    return true;
  }
  return false;
}

void container::setSpaceChild (s_int16 space) {
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

void container::setLayout (layout * l) {
  if (m_layout) delete m_layout;
  m_layout = l;
  if (m_layout) m_layout->setContainer (this);
  updateLayout ();
}

void container::setExtendPolicy (u_int8 extend) {
  m_extend = extend;
  updateLayout ();
  switch (m_extend) {
  case POLICY_EXPAND_CHILD:
  case POLICY_STRETCH_CHILD:
    std::cerr << "Not implemented : " << (u_int8) m_extend << " " << __FILE__ << " " << __LINE__ << std::endl;
    assert (false);
  }
}
