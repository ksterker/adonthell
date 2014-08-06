%module gui
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <adonthell/base/types.h>
#include <adonthell/event/event.h>
#include <adonthell/gui/widget.h>
#include <adonthell/gui/font.h>
#include <adonthell/gui/label.h>
#include <adonthell/gui/button.h>
#include <adonthell/gui/layout.h>
#include <adonthell/gui/listlayout.h>
#include <adonthell/gui/canvas.h>
#include <adonthell/gui/indicatorbar.h>
#include <adonthell/gui/scrollview.h>
#include <adonthell/gui/option.h>
#include <adonthell/gui/textbox.h>
#include <adonthell/gui/ui_event.h>
#include <adonthell/gui/conversation.h>
#include <adonthell/gui/window_manager.h>
#include <adonthell/python/callback.h>

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}

%include "stdint.i"
%include "std_string.i"

// typemap for passing a vector<textsize> as out parameter
namespace gui {
    
    %apply SWIGTYPE *DISOWN { layout *window_disowned };
    %apply SWIGTYPE *DISOWN { widget *child_disowned };
    
    // make sure window_manager takes ownership of windows
    %extend window_manager 
    { 
        static void add(const u_int16 & x, const u_int16 & y, gfx::drawable & content, const gui::fade_type & f = NONE, const gui::window_type & w = DIALOG)
        {
            gui::window_manager::add(x, y, content, f, w);
        }
        
        static void add(gui::window *window_disowned, const gui::fade_type & f = NONE)
        {
            gui::window_manager::add(window_disowned, f);
        }
    }
    %ignore window_manager::add;
    
    // make sure layout takes ownership of children
    %extend layout
    {
        void add_child (gui::widget & child_disowned, const s_int16 & x, const s_int16 & y)
        {
            $self->add_child (child_disowned, x, y);
        }
    }
    %ignore layout::add_child; 

    // make sure list_layout takes ownership of children
    %extend list_layout
    {
        void add_child (gui::widget & child_disowned)
        {
            $self->add_child (child_disowned);
        }
        
        void insert_child (widget & child_disowned, const u_int32 & pos)
        {
            $self->insert_child (child_disowned, pos);
        }
    }
    %ignore list_layout::add_child;
    %ignore list_layout::insert_child;

    // make sure scrollview takes ownership of wrapped container
    %extend scrollview
    {
        void set_child (gui::layout & window_disowned)
        {
            $self->set_child (window_disowned);
        }
    }
    %ignore scrollview::set_child; 

    %typemap(in, numinputs = 0) (std::vector<gui::textsize> &, int & x, int & y) "
        std::vector<gui::textsize> v;
        int x, y;

        $1 = &v;
        $2 = &x;
        $3 = &y;
    "

    %typemap(argout) (std::vector<gui::textsize> & , int & y, int & x) {
        // get rid of SWIGs result
        Py_XDECREF($result);

        // TODO: handle vector

        // return tuple containing x and y value
        $result = Py_BuildValue ("(ii)", (int) $2, (int) $3);
    }
}

%import <adonthell/base/types.h>
%import(module="gfx") <adonthell/gfx/drawable.h>
%import(module="event") <adonthell/event/event.h>

%include <adonthell/gui/widget.h>
%include <adonthell/gui/font.h>
%include <adonthell/gui/label.h>
%include <adonthell/gui/button.h>
%include <adonthell/gui/layout.h>
%include <adonthell/gui/listlayout.h>
%include <adonthell/gui/canvas.h>
%include <adonthell/gui/indicatorbar.h>
%include <adonthell/gui/scrollview.h>
%include <adonthell/gui/option.h>
%include <adonthell/gui/textbox.h>
%include <adonthell/gui/ui_event.h>
%include <adonthell/gui/conversation.h>
%include <adonthell/gui/window.h>
%include <adonthell/gui/window_manager.h>


