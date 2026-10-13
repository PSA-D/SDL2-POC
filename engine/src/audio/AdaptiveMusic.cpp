#include "audio/AdaptiveMusic.h"

#include <algorithm>
#include <utility>

namespace engine::audio
{
AdaptiveMusic::AdaptiveMusic(std::vector<float> layerThresholds)
    : thresholds(std::move(layerThresholds))
    , volumes(thresholds.size(), 0.0F)
{
}

void AdaptiveMusic::setIntensity(float value)
{
    intensity = std::clamp(value, 0.0F, 1.0F);
}

void AdaptiveMusic::update(float dt, AudioBackend& backend)
{
    const float maxStep = FADE_PER_SECOND * dt;
    for (std::size_t layer = 0; layer < volumes.size(); ++layer)
    {
        const float target = intensity >= thresholds[layer] ? 1.0F : 0.0F;
        volumes[layer] += std::clamp(target - volumes[layer], -maxStep, maxStep);
        backend.setLayerVolume(layer, volumes[layer]);
    }
}
} // namespace engine::audio
