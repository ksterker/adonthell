%module gui
%{

#include "base/types.h"
#include "gui/gui.h"

using namespace gui;
%}

%include "std_string.i"

%import "base/types.h"
%import "gfx/drawing_area.h"
%include "gui/base.h"
%include "gui/container.h"
