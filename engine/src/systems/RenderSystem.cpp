#include "systems/RenderSystem.h"

#include "components/Sprite.h"
#include "components/Transform.h"

namespace engine::systems
{
using components::Sprite;
using components::Transform;

void RenderSystem::update(core::Registry& registry, graphics::Renderer& renderer)
{
    registry.each<Transform, Sprite>(
        [&renderer](core::Entity /*entity*/, const Transform& transform, const Sprite& sprite)
        { renderer.fillRect(transform.position, sprite.size, sprite.color); });
}
} // namespace engine::systems
