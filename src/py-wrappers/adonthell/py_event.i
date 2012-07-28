%module event
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include <adonthell/event/date.h>
#include <adonthell/event/factory.h>
#include <adonthell/event/manager.h>
#include <adonthell/event/time_event.h>
#include <adonthell/event/listener_python.h>

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}


%include "stdint.i"
%include "std_string.i"

namespace events {
    // make sure that C++ gets ownership of the event passed to factory::add
    %feature("shadow") factory::add {
        def add(self, ev): 
            ev.thisown = 0
            return _event.factory_add(self, ev)
    }
}

%import <adonthell/base/types.h>
%include <adonthell/event/date.h>
%include <adonthell/event/event.h>
%include <adonthell/event/time_event.h>
%include <adonthell/event/listener.h>
%include <adonthell/event/listener_python.h>
%include <adonthell/event/factory.h>
%include <adonthell/event/manager.h>
