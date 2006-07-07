/*
   $Id: container.cc,v 1.11 2006/07/07 17:34:46 Mithander Exp $

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
  set_extend_policy (POLICY_NORMAL);
}

void container::add_child (base * m)
{
  m_childs.push_back (m);
  
  m->set_parent (this);
  
  m->update_position ();

  // WARNING: I think update layout must be called manually
  // because if there is a lot's of childs it can take a long time ...
  update_layout ();

#ifdef DEBUG
  std::cout << "Nd childs: " << (int) m_childs.size () << std::endl; 
#endif /* DEBUG */
}

void container::remove_child (base * m)
{
  
  m_childs.remove (m);
  
  m->set_parent (NULL);
  
  update_layout ();
#ifdef DEBUG
  std::cout << "Nd childs: " << (int) m_childs.size () << std::endl; 
#endif
}

void container::update_position ()
{
  base::update_position (); // update his position

  for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
    (*i)->update_position ();
}

void container::update_layout ()
{
  // by default no layout ...
  // we can imagine a solution with a pluggable system
  // to set a specific manage
  if (m_layout) {
    m_layout->update ();
    
    // check the policy used
    u_int16 l = get_length (), h = get_height ();
    switch (m_extend) {
    case POLICY_EXPAND_CONTAINER:
      if (m_layout->get_max_length () > l) l = m_layout->get_max_length ();
      if (m_layout->get_max_height () > h) h = m_layout->get_max_height ();
      set_size (l, h);
      update_layout ();
      break;
    
    case POLICY_EXPAND_CHILD:
    case POLICY_STRETCH_CHILD:
      std::cerr << "Not implemented : " << (u_int8) m_extend << " " << __FILE__ << " " << __LINE__ << std::endl;
      assert (false);
    }
  }
}

void container::destroy_all ()
{
  for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
    delete *i;
  m_childs.clear ();
}

bool container::draw_contents (gfx::surface * sf) {
  if (base::draw_contents (sf)) {
    for (ListChild::iterator i = m_childs.begin (); i!= m_childs.end (); ++i)
      (*i)->draw (sf, this);
    return true;
  }
  return false;
}

void container::set_space_child (s_int16 space) {
  m_space_child = space;
  update_layout ();
}

void container::set_space_border (s_int16 space)
{
  m_space_border = space;
  update_layout ();
}

container::~container ()
{
  destroy_all ();
  if (m_parent) m_parent->remove_child (this);
  if (m_layout) delete m_layout;
}

void container::set_layout (layout * l) {
  if (m_layout) delete m_layout;
  m_layout = l;
  if (m_layout) m_layout->set_container (this);
  update_layout ();
}

void container::set_extend_policy (u_int8 extend) {
  m_extend = extend;
  update_layout ();
  switch (m_extend) {
  case POLICY_EXPAND_CHILD:
  case POLICY_STRETCH_CHILD:
    std::cerr << "Not implemented : " << (u_int8) m_extend << " " << __FILE__ << " " << __LINE__ << std::endl;
    assert (false);
  }
}
