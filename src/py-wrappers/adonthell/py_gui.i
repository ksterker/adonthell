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
%import "gfx/drawing_area.h"
%import "input/input.h"
%include "gui/base.h"
%include "gui/layout.h"
%include "gui/listlayout.h"
%include "gui/container.h"
