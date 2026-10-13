#pragma once

#include "core/Registry.h"
#include "graphics/Renderer.h"

namespace engine::systems
{
// Tekent elke entity met Transform + Sprite als gekleurde rechthoek.
class RenderSystem
{
public:
    static void update(core::Registry& registry, graphics::Renderer& renderer);
};
} // namespace engine::systems
