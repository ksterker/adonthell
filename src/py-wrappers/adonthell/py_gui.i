%module gui
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "base/types.h"
#include "event/event.h"
#include "gui/gui.h"
#include "gui/ui_event.h"
#include "gui/conversation.h"
#include "gui/window_manager.h"
#include "python/callback.h"
%}

%include "stdint.i"
%include "std_string.i"

// typemap for passing a vector<textsize> as out parameter
namespace gui {
    %rename (_forbidden_add) window_manager::add;   
    
    %extend window_manager {
        %pythoncode {
            def add (x, y, window, fade = 0):
                window.thisown = 0
                return _gui.window_manager__forbidden_add (x, y, window, fade)
            add = staticmethod(add)
        }
    }

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

%import "base/types.h"
%import(module="gfx") "gfx/drawable.h"
%import(module="event") "event/event.h"

%include "gui/widget.h"
%include "gui/font.h"
%include "gui/label.h"
%include "gui/button.h"
%include "gui/layout.h"
%include "gui/canvas.h"
%include "gui/indicatorbar.h"
%include "gui/scrollview.h"
%include "gui/option.h"
%include "gui/textbox.h"
%include "gui/ui_event.h"
%include "gui/conversation.h"
%include "gui/window_manager.h"


