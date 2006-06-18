%module event
%{

#include <string>
#include "event/date.h"
#include "event/factory.h"
#include "event/manager.h" 
#include "event/time_event.h"
#include "event/listener_python.h"
%}

%include "std_string.i"

namespace events {
    // make sure that C++ gets ownership of the event passed to factory::add
    %feature("shadow") factory::add {
        def add(self, ev): 
            ev.thisown = 0
            return _event.factory_add(self, ev)
    }
}

%include "base/types.h"
%include "event/date.h"
%include "event/event.h"
%include "event/time_event.h"
%include "event/listener.h"
%include "event/listener_python.h"
%include "event/factory.h"
%include "event/manager.h"
