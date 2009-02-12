%module gui
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "base/types.h"
#include "gui/gui.h"

using namespace gui;
%}

%include "stdint.i"
%include "std_string.i"

%import "base/types.h"
%include "gui/base.h"
%include "gui/widget.h"
%include "gui/font.h"
%include "gui/label.h"
%include "gui/button.h"
%include "gui/layout.h"
%include "gui/option.h"
%include "gui/textbox.h"
