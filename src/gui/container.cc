#include "gui/container.h"

using namespace gui;

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


container::~container ()
{
  destroyAll ();
  if (m_parent) m_parent->removeChild (this);
}
