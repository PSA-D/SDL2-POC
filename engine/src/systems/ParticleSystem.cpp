#include "systems/ParticleSystem.h"

#include "components/Particle.h"
#include "components/ParticleEmitter.h"
#include "components/Sprite.h"
#include "components/Transform.h"
#include "components/Velocity.h"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace engine::systems
{
using components::Particle;
using components::ParticleEmitter;
using components::Sprite;
using components::Transform;
using components::Velocity;

namespace
{
constexpr float GRAVITY = 600.0F; // pixels/s², maakt er een fontein van
constexpr utils::Vec2 PARTICLE_SIZE{6.0F, 6.0F};
constexpr float MIN_LIFETIME = 1.0F;
constexpr float MAX_LIFETIME = 2.0F;
} // namespace

void ParticleSystem::update(core::Registry& registry, float dt)
{
    // 1. Verouderen, vallen, vervagen en opruimen.
    registry.each<Particle, Sprite, Velocity>(
        [&registry, dt](core::Entity entity, Particle& particle, Sprite& sprite, Velocity& velocity)
        {
            particle.age += dt;
            velocity.linear.y += GRAVITY * dt;

            const float remaining = std::clamp(1.0F - (particle.age / particle.lifetime), 0.0F, 1.0F);
            sprite.color.a = static_cast<std::uint8_t>(255.0F * remaining);

            if (particle.age >= particle.lifetime)
                registry.destroyLater(entity);
        });

    // 2. Tellen hoeveel er bij moeten. Nog niet aanmaken: dat mag niet tijdens each().
    std::vector<utils::Vec2> spawnPositions;
    registry.each<ParticleEmitter, Transform>(
        [&spawnPositions, dt](core::Entity /*entity*/, ParticleEmitter& emitter, const Transform& transform)
        {
            emitter.accumulator += emitter.ratePerSecond * dt;
            while (emitter.accumulator >= 1.0F)
            {
                spawnPositions.push_back(transform.position);
                emitter.accumulator -= 1.0F;
            }
        });

    // 3. Aanmaken, na de lussen.
    for (const utils::Vec2 position : spawnPositions)
        spawn(registry, position);
}

void ParticleSystem::spawn(core::Registry& registry, utils::Vec2 position)
{
    std::uniform_real_distribution<float> sideways(-120.0F, 120.0F);
    std::uniform_real_distribution<float> upwards(-650.0F, -450.0F);
    std::uniform_real_distribution<float> lifetime(MIN_LIFETIME, MAX_LIFETIME);
    std::uniform_int_distribution<int> green(100, 255);

    const core::Entity particle = registry.create();
    registry.add<Transform>(particle, position - (PARTICLE_SIZE * 0.5F));
    registry.add<Velocity>(particle, utils::Vec2{sideways(random), upwards(random)});
    registry.add<Sprite>(particle, PARTICLE_SIZE, graphics::Color{40, static_cast<std::uint8_t>(green(random)), 255});
    registry.add<Particle>(particle, 0.0F, lifetime(random));
}
} // namespace engine::systems
