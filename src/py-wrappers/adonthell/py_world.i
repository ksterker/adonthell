%module world
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include "base/types.h"
#include "python/callback.h"
#include "world/area_manager.h"
#include "world/object.h"
#include "world/character.h"
#include "world/mapview.h"
#include "world/schedule.h"

using namespace world;

%}

%include "stdint.i"
%include "std_string.i"

%import "base/types.h"
%import "py_gfx.i"

// typemap for returning a string pointer as python string
%typemap(out) std::string * {
    if ($1 == NULL) $result = Py_None;
    else $result = PyString_FromString ($1->c_str ());
}

// typemap for returning a list<chunk_info*> as python list
%typemap(out) std::list<chunk_info*> {
    unsigned int index = 0;
    $result = PyList_New ($1.size ());
    for (std::list<chunk_info*>::const_iterator i = $1.begin (); i != $1.end (); i++)
        PyList_SET_ITEM ($result, index++, python::pass_instance (*i));
}

%typemap(freearg) std::list<chunk_info*> "delete $1;"

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
%include "world/action.h"
%include "world/character.h"
%include "world/chunk.h"
%include "world/chunk_info.h"
%include "world/entity.h"
%include "world/area.h"
%include "world/area_manager.h"
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
    /// get a character from the map
    world::character* get_character (const std::string & name)
    {
        return dynamic_cast<world::character*> (self->get_entity (name));
    }
};

// class extension to handle Python callbacks
%extend world::pathfinding_manager
{
    void set_callback (const s_int16 id, PyObject * callback)
    {
        self->set_callback (id, new python::functor_1<const s_int32>(callback));
    }
};
