%module audio
%feature("autodoc", "1");   // enable docstrings for python wrappers

%{

#include <string>
#include "base/types.h"
#include "audio/audio.h"
#include "audio/sound.h"
#include "python/callback.h"
#include "audio/audio_event.h"
#include "event/time_event.h"

using namespace audio;

%}

%include "stdint.i"
%include "std_string.i"

%import "py_event.i"    // since audio_event inherits from event
%import "base/types.h"

%include "audio/audio.h"
%include "audio/sound.h"
%include "audio/audio_event.h"
