#include "gui/container.h"
#include "gui/layout.h"

using namespace gui;


layout::layout ()
{
  m_container = NULL;
}

bool layout::update ()
{
  return true;
}

void layout::setContainer (container * ct)
{
  m_container = ct;
  if (ct) update ();
}
