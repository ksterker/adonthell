%module audio
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{

#include <string>
#include <adonthell/base/types.h>
#include <adonthell/audio/audio.h>
#include <adonthell/audio/sound.h>
#include <adonthell/python/callback.h>
#include <adonthell/audio/audio_event.h>
#include <adonthell/event/time_event.h>

using namespace audio;

extern "C" {
    void check_module_version (const char *name, const unsigned int & module_ver);
}
%}

%init %{
    check_module_version (SWIG_name, SWIGVERSION);
%}


%include "stdint.i"
%include "std_string.i"

%import "py_event.i"    // since audio_event inherits from event
%import <adonthell/base/types.h>

%include <adonthell/audio/audio.h>
%include <adonthell/audio/sound.h>
%include <adonthell/audio/audio_event.h>
