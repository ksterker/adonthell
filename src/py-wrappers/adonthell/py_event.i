%module event
%{

#include <string>
#include "event/date.h"
#include "event/factory.h"
#include "event/manager.h"
#include "event/time_event.h"
#include "event/time_event_manager.h"
%}

%include "std_string.i"

%include "base/types.h"
%include "event/date.h"
%include "event/event.h"
%include "event/time_event.h"
%include "event/listener.h"
%include "event/factory.h"
%include "event/manager.h"
%include "event/manager_base.h"
%include "event/time_event_manager.h"
