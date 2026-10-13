#include "DemoApp.h"

#include "SoundGenerator.h"
#include "effects/BoxesEffect.h"
#include "effects/ParticleEffect.h"
#include "graphics/Color.h"
#include "systems/BounceSystem.h"
#include "systems/MovementSystem.h"
#include "systems/RenderSystem.h"

#include <algorithm>
#include <string>

namespace game
{
using engine::audio::AudioBackendType;
using engine::input::EventType;
using engine::input::Key;

namespace
{
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
constexpr engine::utils::Vec2 SCREEN_SIZE{static_cast<float>(WIDTH), static_cast<float>(HEIGHT)};
constexpr engine::graphics::Color BACKGROUND{20, 20, 30};
constexpr float BEEP_VOLUME = 0.8F;
constexpr float TITLE_INTERVAL = 0.5F; // seconden tussen titel-updates
} // namespace

DemoApp::DemoApp()
    : window("SDL2 PoC", WIDTH, HEIGHT)
    , renderer(window)
    , beepWav(generateBeep())
    , layerWavs(generateLayers())
    , music({0.0F, 0.33F, 0.66F}) // drempel per laag: de basislaag klinkt altijd
    , mouse(SCREEN_SIZE * 0.5F)
{
    switchEffect(1);
    startAudio(AudioBackendType::SdlCore);
}

void DemoApp::run()
{
    while (running)
    {
        const float dt = clock.tick();
        while (const auto event = engine::input::pollEvent())
            handleEvent(*event);

        effect->update(registry, level, mouse);
        engine::systems::MovementSystem::update(registry, dt);
        engine::systems::BounceSystem::update(registry, SCREEN_SIZE);
        particleSystem.update(registry, dt);
        registry.flush();

        music.setIntensity(effect->intensity(registry));
        music.update(dt, *audio);

        renderer.clear(BACKGROUND);
        engine::systems::RenderSystem::update(registry, renderer);
        renderer.present();

        updateTitle(dt);
    }
}

void DemoApp::handleEvent(const engine::input::Event& event)
{
    if (event.type == EventType::Quit)
    {
        running = false;
        return;
    }
    if (event.type == EventType::MouseMotion)
    {
        mouse = event.mouse;
        return;
    }

    switch (event.key)
    {
    case Key::Num1:
        switchEffect(1);
        break;
    case Key::Num2:
        switchEffect(2);
        break;
    case Key::Up:
        level = std::min(level + 1, MAX_LEVEL);
        break;
    case Key::Down:
        level = std::max(level - 1, 1);
        break;
    case Key::Space:
        audio->playSound(beep, BEEP_VOLUME);
        break;
    case Key::A:
        startAudio(audioType == AudioBackendType::SdlCore ? AudioBackendType::SdlMixer : AudioBackendType::SdlCore);
        break;
    case Key::Escape:
        running = false;
        break;
    case Key::Unknown:
        break;
    }
}

void DemoApp::switchEffect(int number)
{
    registry.clear(); // elk effect begint met een lege registry
    if (number == 1)
        effect = std::make_unique<BoxesEffect>();
    else
        effect = std::make_unique<ParticleEffect>(registry);
}

void DemoApp::startAudio(AudioBackendType type)
{
    audio.reset(); // eerst de oude backend sluiten, zodat er maar één audioapparaat open is
    audio = engine::audio::createAudioBackend(type);
    audioType = type;

    beep = audio->loadSound(beepWav);
    std::vector<engine::audio::SoundId> layers;
    layers.reserve(layerWavs.size());
    for (const auto& wav : layerWavs)
        layers.push_back(audio->loadSound(wav));
    audio->startLayers(layers);
    // De volumes van de lagen zet AdaptiveMusic bij de volgende update().
}

void DemoApp::updateTitle(float dt)
{
    ++frames;
    titleTimer += dt;
    if (titleTimer < TITLE_INTERVAL)
        return;

    const int fps = static_cast<int>(static_cast<float>(frames) / titleTimer);
    const int intensityPercent = static_cast<int>(effect->intensity(registry) * 100.0F);
    window.setTitle(std::string(effect->name()) + " | audio: " + std::string(audio->name()) + " | niveau " +
                    std::to_string(level) + "/" + std::to_string(MAX_LEVEL) + " | intensity " +
                    std::to_string(intensityPercent) + "% | " + std::to_string(fps) + " FPS");
    frames = 0;
    titleTimer = 0.0F;
}
} // namespace game
