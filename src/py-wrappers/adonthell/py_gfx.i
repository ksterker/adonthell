%module gfx
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include "base/types.h"
#include "gfx/gfx.h"
#include "gfx/drawable.h"
#include "gfx/sprite.h"
%}

%include "std_string.i"

%import "base/types.h"
%include "gfx/gfx.h"
%include "gfx/drawing_area.h"
%include "gfx/drawable.h"
%include "gfx/surface.h"
%include "gfx/screen.h"
%include "gfx/sprite.h"
