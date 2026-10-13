#include "audio/sdl2/SdlCoreAudioBackend.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

namespace engine::audio
{
namespace
{
constexpr int SAMPLE_RATE = 44100;
constexpr Uint16 BUFFER_SAMPLES = 1024; // zelfde buffergrootte als SdlMixerAudioBackend
} // namespace

SdlCoreAudioBackend::SdlCoreAudioBackend()
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
        throw std::runtime_error(std::string("SDL_InitSubSystem(AUDIO) mislukt: ") + SDL_GetError());

    SDL_AudioSpec wanted{};
    wanted.freq = SAMPLE_RATE;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = 1;
    wanted.samples = BUFFER_SAMPLES;
    wanted.callback = &SdlCoreAudioBackend::audioCallback;
    wanted.userdata = this;

    // Zonder allowed_changes zet SDL zelf om als het apparaat een ander formaat wil.
    device = SDL_OpenAudioDevice(nullptr, 0, &wanted, nullptr, 0);
    if (device == 0)
    {
        const std::string error = SDL_GetError();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        throw std::runtime_error("SDL_OpenAudioDevice mislukt: " + error);
    }
    SDL_PauseAudioDevice(device, 0); // 0 = afspelen
}

SdlCoreAudioBackend::~SdlCoreAudioBackend()
{
    SDL_CloseAudioDevice(device);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

SoundId SdlCoreAudioBackend::loadSound(std::span<const std::byte> wav)
{
    SDL_AudioSpec spec{};
    Uint8* buffer = nullptr;
    Uint32 length = 0;
    SDL_RWops* stream = SDL_RWFromConstMem(wav.data(), static_cast<int>(wav.size()));
    if (SDL_LoadWAV_RW(stream, 1, &spec, &buffer, &length) == nullptr)
        throw std::runtime_error(std::string("SDL_LoadWAV_RW mislukt: ") + SDL_GetError());

    // Deze eigen mixer zet niets om: het geluid moet al in het formaat van het apparaat zijn.
    if (spec.format != AUDIO_S16SYS || spec.channels != 1 || spec.freq != SAMPLE_RATE)
    {
        SDL_FreeWAV(buffer);
        throw std::runtime_error("SdlCoreAudioBackend verwacht 16-bit mono WAV op 44,1 kHz");
    }

    std::vector<std::int16_t> samples(length / sizeof(std::int16_t));
    std::memcpy(samples.data(), buffer, samples.size() * sizeof(std::int16_t));
    SDL_FreeWAV(buffer);

    SDL_LockAudioDevice(device);
    const SoundId id = sounds.size();
    sounds.push_back(std::move(samples));
    SDL_UnlockAudioDevice(device);
    return id;
}

void SdlCoreAudioBackend::playSound(SoundId sound, float volume)
{
    SDL_LockAudioDevice(device);
    voices.push_back({.sound = sound, .position = 0, .volume = volume});
    SDL_UnlockAudioDevice(device);
}

void SdlCoreAudioBackend::startLayers(std::span<const SoundId> layerSounds)
{
    SDL_LockAudioDevice(device);
    layers.clear();
    for (const SoundId sound : layerSounds)
        layers.push_back({.sound = sound, .volume = 0.0F});
    layerPosition = 0;
    SDL_UnlockAudioDevice(device);
}

void SdlCoreAudioBackend::setLayerVolume(std::size_t layer, float volume)
{
    SDL_LockAudioDevice(device);
    layers[layer].volume = volume;
    SDL_UnlockAudioDevice(device);
}

void SdlCoreAudioBackend::audioCallback(void* userdata, Uint8* stream, int length)
{
    auto* self = static_cast<SdlCoreAudioBackend*>(userdata);
    // Het apparaat is als 16-bit mono geopend, dus de buffer bestaat uit int16-samples.
    auto* samples = reinterpret_cast<std::int16_t*>(stream); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    self->mix({samples, static_cast<std::size_t>(length) / sizeof(std::int16_t)});
}

void SdlCoreAudioBackend::mix(std::span<std::int16_t> output)
{
    for (std::int16_t& sample : output)
    {
        float value = 0.0F;

        for (const Layer& layer : layers)
        {
            const auto& data = sounds[layer.sound];
            value += layer.volume * static_cast<float>(data[layerPosition % data.size()]);
        }
        ++layerPosition;

        for (Voice& voice : voices)
        {
            const auto& data = sounds[voice.sound];
            if (voice.position < data.size())
                value += voice.volume * static_cast<float>(data[voice.position++]);
        }

        sample = static_cast<std::int16_t>(std::clamp(value, -32768.0F, 32767.0F));
    }

    std::erase_if(voices, [this](const Voice& voice) { return voice.position >= sounds[voice.sound].size(); });
}
} // namespace engine::audio
