%module input
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include <adonthell/base/types.h>
#include <adonthell/input/input.h>
#include <adonthell/python/callback.h>

using namespace input;

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
%include <adonthell/input/input.h>
%include <adonthell/input/event.h>
%include <adonthell/input/keyboard_event.h>
%include <adonthell/input/mouse_event.h>
%include <adonthell/input/joystick_event.h>
%include <adonthell/input/control_event.h>
%include <adonthell/input/listener.h>
%include <adonthell/input/manager.h>

// Class extension to handle Python callbacks
%extend input::listener
{
    void connect_keyboard_function(PyObject * f)
    {
        self->connect_keyboard_function(new python::functor_1ret<keyboard_event *, bool>(f));
    }

    void connect_mouse_function(PyObject * f)
    {
        self->connect_mouse_function(new python::functor_1ret<mouse_event *, bool>(f));
    }

    void connect_joystick_function(PyObject * f)
    {
        self->connect_joystick_function(new python::functor_1ret<joystick_event *, bool>(f));
    }

    void connect_control_function(PyObject * f)
    {
        self->connect_control_function(new python::functor_1ret<control_event *, bool>(f));
    }
};
