%module world
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "base/types.h"
#include "world/area.h"
#include "world/object.h"
#include "world/character.h"
#include "world/mapview.h"

using namespace world;

%}

%include "std_string.i"

%import "base/types.h"
%import "py_gfx.i"

// object is an existing python class
%rename(entity) world::object;

%include "world/placeable_model.h"
%include "world/placeable.h"
%include "world/coordinates.h"
%include "world/object.h"
%include "world/moving.h"
%include "world/character.h"
%include "world/square.h"
%include "world/area.h"
%include "world/placeable_shape.h"
%include "world/vector3.h"
%include "world/chunk.h"
%include "world/mapview.h"

%template(vector3i) world::vector3<s_int32>;
