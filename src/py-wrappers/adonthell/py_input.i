%module input
%{

#include <string>
#include "base/types.h"
#include "input/input.h"
#include "python/callback.h"

using namespace input;

%}

%include "std_string.i"

%import "base/types.h"
%include "input/input.h"
%include "input/event.h"
%include "input/keyboard_event.h"
%include "input/mouse_event.h"
%include "input/joystick_event.h"
%include "input/control_event.h"
%include "input/listener.h"
%include "input/manager.h"

// Class extension to handle Python callbacks
%extend input::listener
{
    void connect_keyboard_function(PyObject * f)
    {
        self->connect_keyboard_function(new python::functor_1ret<keyboard_event *, int>(f));
    }

    void connect_mouse_function(PyObject * f)
    {
        self->connect_mouse_function(new python::functor_1ret<mouse_event *, int>(f));
    }

    void connect_joystick_function(PyObject * f)
    {
        self->connect_joystick_function(new python::functor_1ret<joystick_event *, int>(f));
    }

    void connect_control_function(PyObject * f)
    {
        self->connect_control_function(new python::functor_1ret<control_event *, int>(f));
    }
};
