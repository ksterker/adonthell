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
