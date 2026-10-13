#pragma once

#include "audio/AudioBackend.h"

#include <memory>

namespace engine::audio
{
enum class AudioBackendType
{
    SdlCore,
    SdlMixer,
};

// De game kiest een backend zonder de SDL-headers van de implementaties te zien.
std::unique_ptr<AudioBackend> createAudioBackend(AudioBackendType type);
} // namespace engine::audio
