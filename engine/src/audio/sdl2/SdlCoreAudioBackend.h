#pragma once

#include "audio/AudioBackend.h"

#include <SDL.h>
#include <cstdint>
#include <vector>

namespace engine::audio
{
// Kern-SDL2: SDL roept audioCallback() aan op de audiothread en wij mixen alle geluiden zelf.
class SdlCoreAudioBackend final : public AudioBackend
{
public:
    SdlCoreAudioBackend();
    ~SdlCoreAudioBackend() override;

    SdlCoreAudioBackend(const SdlCoreAudioBackend&) = delete;
    SdlCoreAudioBackend& operator=(const SdlCoreAudioBackend&) = delete;
    SdlCoreAudioBackend(SdlCoreAudioBackend&&) = delete;
    SdlCoreAudioBackend& operator=(SdlCoreAudioBackend&&) = delete;

    [[nodiscard]] std::string_view name() const override
    {
        return "SDL2 core";
    }

    SoundId loadSound(std::span<const std::byte> wav) override;
    void playSound(SoundId sound, float volume) override;
    void startLayers(std::span<const SoundId> layerSounds) override;
    void setLayerVolume(std::size_t layer, float volume) override;

private:
    struct Voice
    {
        SoundId sound{0};
        std::size_t position{0};
        float volume{1.0F};
    };

    struct Layer
    {
        SoundId sound{0};
        float volume{0.0F};
    };

    static void audioCallback(void* userdata, Uint8* stream, int length);
    void mix(std::span<std::int16_t> output);

    SDL_AudioDeviceID device{0};

    // Alles hieronder wordt ook op de audiothread gelezen: alleen aanpassen met SDL_LockAudioDevice.
    std::vector<std::vector<std::int16_t>> sounds;
    std::vector<Voice> voices;    // losse geluidseffecten die nog spelen
    std::vector<Layer> layers;    // adaptieve lagen, lopen in een lus
    std::size_t layerPosition{0}; // één gedeelde positie, dus alle lagen lopen exact gelijk
};
} // namespace engine::audio
