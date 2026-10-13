#include "effects/BoxesEffect.h"

#include "components/Sprite.h"
#include "components/Transform.h"
#include "components/Velocity.h"

#include <cstddef>
#include <cstdint>

namespace game
{
using engine::components::Sprite;
using engine::components::Transform;
using engine::components::Velocity;
using engine::utils::Vec2;

namespace
{
constexpr std::size_t BOXES_PER_LEVEL = 20;
constexpr Vec2 BOX_SIZE{24.0F, 24.0F};
constexpr float MAX_SPEED = 300.0F; // pixels per seconde
} // namespace

void BoxesEffect::update(engine::core::Registry& registry, int level, Vec2 mouse)
{
    const std::size_t target = static_cast<std::size_t>(level) * BOXES_PER_LEVEL;

    while (boxes.size() < target)
        boxes.push_back(spawnBox(registry, mouse));

    while (boxes.size() > target)
    {
        registry.destroyLater(boxes.back());
        boxes.pop_back();
    }
}

float BoxesEffect::intensity(const engine::core::Registry& registry) const
{
    const std::size_t maximum = static_cast<std::size_t>(MAX_LEVEL) * BOXES_PER_LEVEL;
    return static_cast<float>(registry.count<Sprite>()) / static_cast<float>(maximum);
}

engine::core::Entity BoxesEffect::spawnBox(engine::core::Registry& registry, Vec2 position)
{
    std::uniform_real_distribution<float> speed(-MAX_SPEED, MAX_SPEED);
    std::uniform_int_distribution<int> channel(64, 255);
    const auto randomChannel = [&] { return static_cast<std::uint8_t>(channel(random)); };

    const engine::core::Entity box = registry.create();
    registry.add<Transform>(box, position - (BOX_SIZE * 0.5F));
    registry.add<Velocity>(box, Vec2{speed(random), speed(random)});
    registry.add<Sprite>(box, BOX_SIZE, engine::graphics::Color{randomChannel(), randomChannel(), randomChannel()});
    return box;
}
} // namespace game
