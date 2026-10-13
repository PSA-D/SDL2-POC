#pragma once

#include "audio/AudioBackend.h"

#include <SDL_mixer.h>
#include <memory>
#include <vector>

namespace engine::audio
{
// SDL2_mixer: elk geluid speelt op een eigen kanaal, het mixen en de audiothread regelt de library.
class SdlMixerAudioBackend final : public AudioBackend
{
public:
    SdlMixerAudioBackend();
    ~SdlMixerAudioBackend() override;

    SdlMixerAudioBackend(const SdlMixerAudioBackend&) = delete;
    SdlMixerAudioBackend& operator=(const SdlMixerAudioBackend&) = delete;
    SdlMixerAudioBackend(SdlMixerAudioBackend&&) = delete;
    SdlMixerAudioBackend& operator=(SdlMixerAudioBackend&&) = delete;

    [[nodiscard]] std::string_view name() const override
    {
        return "SDL2_mixer";
    }

    SoundId loadSound(std::span<const std::byte> wav) override;
    void playSound(SoundId sound, float volume) override;
    void startLayers(std::span<const SoundId> layerSounds) override;
    void setLayerVolume(std::size_t layer, float volume) override;

private:
    std::vector<std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>> chunks;
};
} // namespace engine::audio
