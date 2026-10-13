#pragma once

#include "audio/AudioBackend.h"

#include <vector>

namespace engine::audio
{
// Backend-onafhankelijke adaptieve muziek: elke laag heeft een drempel en klinkt zodra de intensity daarboven
// komt. Volumes bewegen met een maximale fade-snelheid, zodat lagen in- en uitfaden in plaats van springen.
class AdaptiveMusic
{
public:
    explicit AdaptiveMusic(std::vector<float> layerThresholds);

    void setIntensity(float value); // 0..1

    // Werkt de volumes bij en geeft ze door aan de backend.
    void update(float dt, AudioBackend& backend);

private:
    static constexpr float FADE_PER_SECOND = 0.5F; // van stil naar vol in 2 seconden

    std::vector<float> thresholds;
    std::vector<float> volumes;
    float intensity{0.0F};
};
} // namespace engine::audio
