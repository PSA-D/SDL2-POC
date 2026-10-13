#pragma once

#include "core/Registry.h"
#include "utils/Math.h"

namespace engine::systems
{
// Houdt entities met Transform + Velocity + Sprite binnen het venster door hun snelheid om te keren.
class BounceSystem
{
public:
    static void update(core::Registry& registry, utils::Vec2 bounds);
};
} // namespace engine::systems
