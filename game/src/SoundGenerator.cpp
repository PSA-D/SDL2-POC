#include "SoundGenerator.h"

#include <cmath>
#include <cstdint>
#include <numbers>
#include <string_view>

namespace game
{
namespace
{
constexpr std::uint32_t SAMPLE_RATE = 44100;
constexpr float AMPLITUDE = 0.25F * 32767.0F; // ruimte laten voor meerdere geluiden tegelijk
constexpr float DECAY = 12.0F;                // hoe snel een aanslag uitsterft
constexpr float LOOP_SECONDS = 2.0F;

// Sinustoon die elke `beatSeconds` opnieuw aanslaat en daarna uitsterft.
std::vector<std::int16_t> pluckedTone(float frequency, float seconds, float beatSeconds)
{
    const auto count = static_cast<std::size_t>(seconds * static_cast<float>(SAMPLE_RATE));
    std::vector<std::int16_t> samples(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        const float time = static_cast<float>(i) / static_cast<float>(SAMPLE_RATE);
        const float envelope = std::exp(-std::fmod(time, beatSeconds) * DECAY);
        const float wave = std::sin(2.0F * std::numbers::pi_v<float> * frequency * time);
        samples[i] = static_cast<std::int16_t>(AMPLITUDE * envelope * wave);
    }
    return samples;
}

// Minimale WAV-header (PCM, 16-bit, mono) gevolgd door de samples, little-endian.
std::vector<std::byte> toWav(const std::vector<std::int16_t>& samples)
{
    std::vector<std::byte> wav;
    const auto writeNumber = [&wav](std::uint32_t value, int byteCount)
    {
        for (int i = 0; i < byteCount; ++i)
            wav.push_back(static_cast<std::byte>((value >> (8 * i)) & 0xFFU));
    };
    const auto writeText = [&wav](std::string_view text)
    {
        for (const char character : text)
            wav.push_back(static_cast<std::byte>(character));
    };

    const auto dataSize = static_cast<std::uint32_t>(samples.size() * sizeof(std::int16_t));
    writeText("RIFF");
    writeNumber(36 + dataSize, 4);
    writeText("WAVE");
    writeText("fmt ");
    writeNumber(16, 4);              // grootte van dit blok
    writeNumber(1, 2);               // PCM
    writeNumber(1, 2);               // mono
    writeNumber(SAMPLE_RATE, 4);     // samples per seconde
    writeNumber(SAMPLE_RATE * 2, 4); // bytes per seconde
    writeNumber(2, 2);               // bytes per sample
    writeNumber(16, 2);              // bits per sample
    writeText("data");
    writeNumber(dataSize, 4);
    for (const std::int16_t sample : samples)
        writeNumber(static_cast<std::uint16_t>(sample), 2);
    return wav;
}
} // namespace

std::vector<std::byte> generateBeep()
{
    return toWav(pluckedTone(880.0F, 0.2F, 0.2F));
}

std::vector<std::vector<std::byte>> generateLayers()
{
    // Een geheel aantal tellen per lus, zodat de lus naadloos herhaalt.
    return {
        toWav(pluckedTone(110.0F, LOOP_SECONDS, 0.5F)),
        toWav(pluckedTone(330.0F, LOOP_SECONDS, 0.25F)),
        toWav(pluckedTone(660.0F, LOOP_SECONDS, 0.125F)),
    };
}
} // namespace game
