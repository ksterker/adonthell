%module event
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{
#include <string>
#include "event/date.h"
#include "event/factory.h"
#include "event/manager.h" 
#include "event/time_event.h"
#include "event/listener_python.h"

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

%import "base/types.h"
%include "event/date.h"
%include "event/event.h"
%include "event/time_event.h"
%include "event/listener.h"
%include "event/listener_python.h"
%include "event/factory.h"
%include "event/manager.h"
