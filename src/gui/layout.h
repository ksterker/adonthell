#ifndef GUI_LAYOUT_H_
#define GUI_LAYOUT_H_

namespace gui
{
  class container;
  
  class layout
    {
    public:
      layout ();
      
      virtual bool update ();
      
      void setContainer (container * ct);

    protected:

      container * m_container;
    };
}

#endif
