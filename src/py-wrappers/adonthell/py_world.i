%module world
%{

#include <string>
#include "base/types.h"
#include "world/area.h"

using namespace world;

%}

%include "std_string.i"

%import "base/types.h"
%import "gfx/drawable.h"
%import "gfx/animation.h"
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
%include "world/placeable_area_gfx.h"
