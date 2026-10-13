#pragma once

#include "core/Registry.h"
#include "utils/Math.h"

#include <random>

namespace engine::systems
{
// Spawnt particles bij elke ParticleEmitter, laat ze vallen en vervagen en ruimt ze op na hun levensduur.
// Bewegen en tekenen doen MovementSystem en RenderSystem, net als bij elke andere entity.
class ParticleSystem
{
public:
    void update(core::Registry& registry, float dt);

private:
    void spawn(core::Registry& registry, utils::Vec2 position);

    std::mt19937 random{std::random_device{}()};
};
} // namespace engine::systems
