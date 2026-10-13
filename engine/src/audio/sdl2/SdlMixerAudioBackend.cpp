#include "audio/sdl2/SdlMixerAudioBackend.h"

#include <SDL.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

namespace engine::audio
{
namespace
{
constexpr int SAMPLE_RATE = 44100;
constexpr int BUFFER_SAMPLES = 1024; // zelfde buffergrootte als SdlCoreAudioBackend

// SDL2_mixer werkt met een volume van 0 tot en met 128.
int toMixVolume(float volume)
{
    return static_cast<int>(std::lround(std::clamp(volume, 0.0F, 1.0F) * MIX_MAX_VOLUME));
}
} // namespace

SdlMixerAudioBackend::SdlMixerAudioBackend()
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
        throw std::runtime_error(std::string("SDL_InitSubSystem(AUDIO) mislukt: ") + SDL_GetError());

    // Zelfde formaat als SdlCoreAudioBackend (16-bit mono), zodat de vergelijking eerlijk is.
    if (Mix_OpenAudio(SAMPLE_RATE, AUDIO_S16SYS, 1, BUFFER_SAMPLES) != 0)
    {
        const std::string error = Mix_GetError();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        throw std::runtime_error("Mix_OpenAudio mislukt: " + error);
    }
}

SdlMixerAudioBackend::~SdlMixerAudioBackend()
{
    Mix_HaltChannel(-1);
    chunks.clear(); // chunks vrijgeven vóór Mix_CloseAudio
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

SoundId SdlMixerAudioBackend::loadSound(std::span<const std::byte> wav)
{
    SDL_RWops* stream = SDL_RWFromConstMem(wav.data(), static_cast<int>(wav.size()));
    Mix_Chunk* chunk = Mix_LoadWAV_RW(stream, 1);
    if (chunk == nullptr)
        throw std::runtime_error(std::string("Mix_LoadWAV_RW mislukt: ") + Mix_GetError());

    chunks.emplace_back(chunk, &Mix_FreeChunk);
    return chunks.size() - 1;
}

void SdlMixerAudioBackend::playSound(SoundId sound, float volume)
{
    // -1 = eerste vrije kanaal. Zijn alle kanalen bezet (standaard 8), dan valt het geluid weg.
    const int channel = Mix_PlayChannel(-1, chunks[sound].get(), 0);
    if (channel >= 0)
        Mix_Volume(channel, toMixVolume(volume));
}

void SdlMixerAudioBackend::startLayers(std::span<const SoundId> layerSounds)
{
    // Laag i speelt op kanaal i. Die kanalen reserveren we, zodat playSound() ze niet afpakt.
    Mix_ReserveChannels(static_cast<int>(layerSounds.size()));
    for (std::size_t i = 0; i < layerSounds.size(); ++i)
    {
        const int channel = static_cast<int>(i);
        Mix_Volume(channel, 0);
        Mix_PlayChannel(channel, chunks[layerSounds[i]].get(), -1); // -1 = eindeloos herhalen
    }
}

void SdlMixerAudioBackend::setLayerVolume(std::size_t layer, float volume)
{
    Mix_Volume(static_cast<int>(layer), toMixVolume(volume));
}
} // namespace engine::audio
