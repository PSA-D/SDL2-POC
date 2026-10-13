#include "systems/BounceSystem.h"

#include "components/Sprite.h"
#include "components/Transform.h"
#include "components/Velocity.h"

#include <cmath>

namespace engine::systems
{
using components::Sprite;
using components::Transform;
using components::Velocity;

namespace
{
// Eén as: buiten [0, max] terugzetten en de snelheid naar binnen laten wijzen.
void bounce(float& position, float& velocity, float max)
{
    if (position < 0.0F)
    {
        position = 0.0F;
        velocity = std::abs(velocity);
    }
    else if (position > max)
    {
        position = max;
        velocity = -std::abs(velocity);
    }
}
} // namespace

void BounceSystem::update(core::Registry& registry, utils::Vec2 bounds)
{
    registry.each<Transform, Velocity, Sprite>(
        [bounds](core::Entity /*entity*/, Transform& transform, Velocity& velocity, const Sprite& sprite)
        {
            bounce(transform.position.x, velocity.linear.x, bounds.x - sprite.size.x);
            bounce(transform.position.y, velocity.linear.y, bounds.y - sprite.size.y);
        });
}
} // namespace engine::systems
