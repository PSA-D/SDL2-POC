#pragma once

#include "core/Registry.h"
#include "utils/Math.h"

#include <string_view>

namespace game
{
constexpr int MAX_LEVEL = 10;

// Een effect zet alleen entities klaar; het echte werk doen de systems van de engine.
class Effect
{
public:
    Effect() = default;
    virtual ~Effect() = default;
    Effect(const Effect&) = delete;
    Effect& operator=(const Effect&) = delete;
    Effect(Effect&&) = delete;
    Effect& operator=(Effect&&) = delete;

    [[nodiscard]] virtual std::string_view name() const = 0;

    // Brengt de entities in lijn met het niveau (1..MAX_LEVEL) en de muispositie.
    virtual void update(engine::core::Registry& registry, int level, engine::utils::Vec2 mouse) = 0;

    // 0..1: aantal entities van het effect ten opzichte van het maximum. Stuurt de adaptieve muziek.
    [[nodiscard]] virtual float intensity(const engine::core::Registry& registry) const = 0;
};
} // namespace game
