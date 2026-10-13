#pragma once

namespace engine::utils
{
// Alleen wat de PoC nodig heeft. Voor clamp/lerp gebruiken we std::clamp en std::lerp.
struct Vec2
{
    float x{0.0F};
    float y{0.0F};
};

constexpr Vec2 operator+(Vec2 a, Vec2 b)
{
    return {a.x + b.x, a.y + b.y};
}

constexpr Vec2 operator-(Vec2 a, Vec2 b)
{
    return {a.x - b.x, a.y - b.y};
}

constexpr Vec2 operator*(Vec2 v, float scalar)
{
    return {v.x * scalar, v.y * scalar};
}
} // namespace engine::utils
