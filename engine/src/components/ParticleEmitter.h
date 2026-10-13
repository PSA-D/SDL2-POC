#pragma once

namespace engine::components
{
struct ParticleEmitter
{
    float ratePerSecond{0.0F};
    float accumulator{0.0F}; // fractie van een particle die nog gespawnd moet worden
};
} // namespace engine::components
