#pragma once

#include "effects/Effect.h"

#include <random>
#include <vector>

namespace game
{
// Stuiterende vierkanten: Transform + Velocity + Sprite, verder doen MovementSystem en BounceSystem het werk.
class BoxesEffect final : public Effect
{
public:
    [[nodiscard]] std::string_view name() const override
    {
        return "Stuiterende vierkanten";
    }

    void update(engine::core::Registry& registry, int level, engine::utils::Vec2 mouse) override;
    [[nodiscard]] float intensity(const engine::core::Registry& registry) const override;

private:
    engine::core::Entity spawnBox(engine::core::Registry& registry, engine::utils::Vec2 position);

    std::vector<engine::core::Entity> boxes;
    std::mt19937 random{std::random_device{}()};
};
} // namespace game
