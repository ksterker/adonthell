%module base
%{

#include <string>
#include "base/types.h"
#include "base/callback.h"
#include "base/timer.h"

%}

%include "std_string.i"

%include "base/types.h"
%include "base/timer.h"
// %include "callback.h"
