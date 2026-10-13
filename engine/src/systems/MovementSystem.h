#pragma once

#include "core/Registry.h"

namespace engine::systems
{
// Beweegt elke entity met Transform + Velocity, of het nu een vierkant of een particle is.
class MovementSystem
{
public:
    static void update(core::Registry& registry, float dt);
};
} // namespace engine::systems
