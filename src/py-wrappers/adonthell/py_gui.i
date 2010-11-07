%module gui
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "base/types.h"
#include "gui/gui.h"
#include "gui/conversation.h"
#include "gui/window_manager.h"
#include "python/callback.h"
%}

%include "stdint.i"
%include "std_string.i"

// typemap for passing a vector<textsize> as out parameter
namespace gui {
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

%include "gui/widget.h"
%include "gui/font.h"
%include "gui/label.h"
%include "gui/button.h"
%include "gui/layout.h"
%include "gui/scrollview.h"
%include "gui/option.h"
%include "gui/textbox.h"
%include "gui/conversation.h"
%include "gui/window_manager.h"

namespace gui
{
    // class extension to handle Python callbacks
    %extend conversation
    {
        conversation(rpg::character & d, int w, int h, PyObject *callback)
        {
            return new gui::conversation(d, w, h, new python::functor_0 (callback));
        }
    };
}
