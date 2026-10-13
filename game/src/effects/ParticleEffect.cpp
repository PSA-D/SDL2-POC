#include "effects/ParticleEffect.h"

#include "components/Particle.h"
#include "components/ParticleEmitter.h"
#include "components/Transform.h"

namespace game
{
using engine::components::Particle;
using engine::components::ParticleEmitter;
using engine::components::Transform;

namespace
{
constexpr float RATE_PER_LEVEL = 60.0F; // particles per seconde per niveau
// Bij het hoogste niveau leven er gemiddeld rate * 1,5 s (gemiddelde levensduur in ParticleSystem) particles.
constexpr float MAX_PARTICLES = static_cast<float>(MAX_LEVEL) * RATE_PER_LEVEL * 1.5F;
} // namespace

ParticleEffect::ParticleEffect(engine::core::Registry& registry)
    : emitter(registry.create())
{
    registry.add<Transform>(emitter);
    registry.add<ParticleEmitter>(emitter);
}

void ParticleEffect::update(engine::core::Registry& registry, int level, engine::utils::Vec2 mouse)
{
    registry.get<Transform>(emitter).position = mouse;
    registry.get<ParticleEmitter>(emitter).ratePerSecond = static_cast<float>(level) * RATE_PER_LEVEL;
}

float ParticleEffect::intensity(const engine::core::Registry& registry) const
{
    return static_cast<float>(registry.count<Particle>()) / MAX_PARTICLES;
}
} // namespace game
