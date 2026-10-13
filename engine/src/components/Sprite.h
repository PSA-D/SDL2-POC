#pragma once

#include "graphics/Color.h"
#include "utils/Math.h"

namespace engine::components
{
struct Sprite
{
    utils::Vec2 size;
    graphics::Color color;
};
} // namespace engine::components
