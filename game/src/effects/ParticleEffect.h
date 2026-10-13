#pragma once

#include "effects/Effect.h"

namespace game
{
// Particle-fontein: één emitter-entity onder de muis, ParticleSystem maakt en verwijdert de particles.
class ParticleEffect final : public Effect
{
public:
    explicit ParticleEffect(engine::core::Registry& registry);

    [[nodiscard]] std::string_view name() const override
    {
        return "Particle-fontein";
    }

    void update(engine::core::Registry& registry, int level, engine::utils::Vec2 mouse) override;
    [[nodiscard]] float intensity(const engine::core::Registry& registry) const override;

private:
    engine::core::Entity emitter;
};
} // namespace game
