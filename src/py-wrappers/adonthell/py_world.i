%module world
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <adonthell/base/types.h>
#include <adonthell/python/callback.h>
#include <adonthell/world/area_manager.h>
#include <adonthell/world/object.h>
#include <adonthell/world/character.h>
#include <adonthell/world/mapview.h>
#include <adonthell/world/schedule.h>
#include <adonthell/world/move_event.h>

using namespace world;

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}

%include "stdint.i"
%include "std_string.i"

%import <adonthell/base/types.h>
%import(module="event") <adonthell/event/event.h>
%import(module="gfx") <adonthell/gfx/drawable.h>

// typemap for returning a string pointer as python string
%typemap(out) std::string * {
    if ($1 == NULL) {
        Py_INCREF(Py_None);
        $result = Py_None;
    }
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

// typemap for returning a singular chunk_info pointer
%typemap(out) chunk_info * {
    if ($1 == NULL)
    {
        Py_INCREF(Py_None);
        $result = Py_None;
    }
    else $result = python::pass_instance($1);
}

// object is an existing python class
%rename(mapobject) world::object;

%include <adonthell/world/vector3.h>

%template(vector3i) world::vector3<s_int32>;

%include <adonthell/python/script.h>
%include <adonthell/world/placeable_model.h>
%include <adonthell/world/placeable.h>
%include <adonthell/world/object.h>
%include <adonthell/world/coordinates.h>
%include <adonthell/world/move_event.h>
%include <adonthell/world/moving.h>
%include <adonthell/world/action.h>
%include <adonthell/world/character.h>
%include <adonthell/world/chunk.h>
%include <adonthell/world/chunk_info.h>
%include <adonthell/world/entity.h>
%include <adonthell/world/area.h>
%include <adonthell/world/area_manager.h>
%include <adonthell/world/pathfinding_manager.h>
%include <adonthell/world/placeable_shape.h>
%include <adonthell/world/renderer.h>
%include <adonthell/world/mapview.h>
%include <adonthell/world/schedule.h>

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
