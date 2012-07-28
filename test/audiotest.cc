#include <adonthell/audio/audio.h>
#include <adonthell/audio/audio_manager.h>
#include <adonthell/base/configuration.h>
#include <adonthell/base/logging.h>

#include <iostream>

using namespace audio;

int main(int argc, char * argv[]) {
    base::configuration cfg;

    google::InitGoogleLogging(argv[0]);

    setup(cfg);

    std::cerr << "audio_buffers: "
              << audio_manager::get_audio_buffers ()
              << std::endl;
    std::cerr << "audio_channels: "
              << audio_manager::get_audio_channels ()
              << std::endl;
    std::cerr << "audio_format: "
              << audio_manager::get_audio_format ()
              << std::endl;
    std::cerr << "audio_mixchannels: "
              << audio_manager::get_audio_mixchannels ()
              << std::endl;
    std::cerr << "audio_rate: "
              << audio_manager::get_audio_rate ()
              << std::endl;

    return 0;
}
