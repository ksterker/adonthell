%module gfx
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include "base/types.h"
#include "gfx/gfx.h"
#include "gfx/drawable.h"
#include "gfx/sprite.h"

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}

%include "stdint.i"
%include "std_string.i"

%import "base/types.h"
%include "gfx/gfx.h"
%include "gfx/drawing_area.h"
%include "gfx/drawable.h"
%include "gfx/surface.h"
%include "gfx/screen.h"
%include "gfx/sprite.h"
%include "gfx/surface_cacher.h"
