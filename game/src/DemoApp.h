#pragma once

#include "audio/AdaptiveMusic.h"
#include "audio/AudioBackend.h"
#include "audio/AudioFactory.h"
#include "core/Clock.h"
#include "core/Registry.h"
#include "effects/Effect.h"
#include "graphics/Renderer.h"
#include "graphics/Window.h"
#include "input/Input.h"
#include "systems/ParticleSystem.h"
#include "utils/Math.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace game
{
// Venster, gameloop, effect wisselen (1/2) en audio-backend wisselen (A).
class DemoApp
{
public:
    DemoApp();

    void run();

private:
    void handleEvent(const engine::input::Event& event);
    void switchEffect(int number);
    void startAudio(engine::audio::AudioBackendType type);
    void updateTitle(float dt);

    engine::graphics::Window window;
    engine::graphics::Renderer renderer;
    engine::core::Registry registry;
    engine::core::Clock clock;
    engine::systems::ParticleSystem particleSystem;
    std::unique_ptr<Effect> effect;

    std::vector<std::byte> beepWav;
    std::vector<std::vector<std::byte>> layerWavs;
    engine::audio::AudioBackendType audioType{engine::audio::AudioBackendType::SdlCore};
    std::unique_ptr<engine::audio::AudioBackend> audio;
    engine::audio::SoundId beep{0};
    engine::audio::AdaptiveMusic music;

    engine::utils::Vec2 mouse;
    int level{3};
    bool running{true};
    float titleTimer{0.0F};
    int frames{0};
};
} // namespace game
