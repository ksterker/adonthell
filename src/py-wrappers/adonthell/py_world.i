%module world
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "base/types.h"
#include "world/area.h"
#include "world/cube3.h"

using namespace world;

%}

%include "std_string.i"

%import "base/types.h"
%import "py_gfx.i"

%include "world/placeable_model.h"
%include "world/placeable.h"
%include "world/placeable_model_gfx.h"
%include "world/coordinates.h"
%include "world/object.h"
%include "world/placeable_gfx.h"
%include "world/moving.h"
%include "world/object_with_gfx.h"
%include "world/character.h"
%include "world/character_with_gfx.h"
%include "world/square.h"
%include "world/area.h"
%include "world/placeable_area.h"
%include "world/vector3.h"
%include "world/triangle3.h"
%include "world/cube3.h"