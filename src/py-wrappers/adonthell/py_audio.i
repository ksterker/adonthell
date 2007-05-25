%module audio
%{

#include <string>
#include "base/types.h"
#include "audio/audio.h"
#include "audio/sound.h"
#include "python/callback.h"
#include "audio/audio_event.h"

using namespace audio;

%}

%include "std_string.i"

%import "base/types.h"
%import "event/event.h"
%include "audio/audio.h"
%include "audio/sound.h"
%include "audio/audio_event.h"
