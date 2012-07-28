%module gfx
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include <adonthell/base/types.h>
#include <adonthell/gfx/gfx.h>
#include <adonthell/gfx/drawable.h>
#include <adonthell/gfx/sprite.h>

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
%include <adonthell/gfx/gfx.h>
%include <adonthell/gfx/drawing_area.h>
%include <adonthell/gfx/drawable.h>
%include <adonthell/gfx/surface.h>
%include <adonthell/gfx/screen.h>
%include <adonthell/gfx/sprite.h>
%include <adonthell/gfx/surface_cacher.h>
