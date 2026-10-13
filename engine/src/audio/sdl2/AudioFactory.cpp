#include "audio/AudioFactory.h"

#include "audio/sdl2/SdlCoreAudioBackend.h"
#include "audio/sdl2/SdlMixerAudioBackend.h"

namespace engine::audio
{
std::unique_ptr<AudioBackend> createAudioBackend(AudioBackendType type)
{
    if (type == AudioBackendType::SdlMixer)
        return std::make_unique<SdlMixerAudioBackend>();
    return std::make_unique<SdlCoreAudioBackend>();
}
} // namespace engine::audio
