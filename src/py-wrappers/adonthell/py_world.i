%module world
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "base/types.h"
#include "python/callback.h"
#include "world/area.h"
#include "world/object.h"
#include "world/character.h"
#include "world/mapview.h"
#include "world/schedule.h"
#include "world/pathfinding_manager.h"

using namespace world;

%}

%include "stdint.i"
%include "std_string.i"

%import "base/types.h"
%import "py_gfx.i"

// object is an existing python class
%rename(mapobject) world::object;

%include "world/vector3.h"

%template(vector3i) world::vector3<s_int32>;

%include "python/script.h"
%include "world/placeable_model.h"
%include "world/placeable.h"
%include "world/object.h"
%include "world/coordinates.h"
%include "world/moving.h"
%include "world/character.h"
%include "world/chunk.h"
%include "world/area.h"
%include "world/pathfinding_manager.h"
%include "world/placeable_shape.h"
%include "world/renderer.h"
%include "world/mapview.h"
%include "world/schedule.h"

%pythoncode %{
class coordinates (vector3i):
    pass
%}

/// no downcasting in python, so we have to improvise ...
%extend world::area {
    /// add an object to the map
    world::object* add_object ()
    {
        return dynamic_cast<world::object*> (self->add_entity (world::OBJECT));
    }

    /// add a character to the map
    world::character* add_character (const std::string & name)
    {
        return dynamic_cast<world::character*> (self->add_entity (world::CHARACTER, name));
    }

    /// get a character from the map
    world::character* get_character (const std::string & name)
    {
        return dynamic_cast<world::character*> (self->get_entity (name));
    }
};

// class extension to handle Python callbacks
%extend world::pathfinding_manager
{
    static void set_callback (const s_int16 id, PyObject * callback)
    {
        world::pathfinding_manager::set_callback (id, new python::functor_1<s_int32>(callback));
    }
};
