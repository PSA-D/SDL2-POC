#include "systems/MovementSystem.h"

#include "components/Transform.h"
#include "components/Velocity.h"

namespace engine::systems
{
using components::Transform;
using components::Velocity;

void MovementSystem::update(core::Registry& registry, float dt)
{
    registry.each<Transform, Velocity>([dt](core::Entity /*entity*/, Transform& transform, const Velocity& velocity)
                                       { transform.position = transform.position + velocity.linear * dt; });
}
} // namespace engine::systems
