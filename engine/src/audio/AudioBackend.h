#pragma once

#include <cstddef>
#include <span>
#include <string_view>

namespace engine::audio
{
using SoundId = std::size_t;

// Runtime-interface (geen compile-time keuze zoals bij graphics), omdat de demo tijdens het draaien wisselt.
class AudioBackend
{
public:
    AudioBackend() = default;
    virtual ~AudioBackend() = default;
    AudioBackend(const AudioBackend&) = delete;
    AudioBackend& operator=(const AudioBackend&) = delete;
    AudioBackend(AudioBackend&&) = delete;
    AudioBackend& operator=(AudioBackend&&) = delete;

    [[nodiscard]] virtual std::string_view name() const = 0;

    virtual SoundId loadSound(std::span<const std::byte> wav) = 0;
    virtual void playSound(SoundId sound, float volume) = 0;       // normaal geluid
    virtual void startLayers(std::span<const SoundId> layers) = 0; // adaptief, loopend
    virtual void setLayerVolume(std::size_t layer, float volume) = 0;
};
} // namespace engine::audio
