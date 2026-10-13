#pragma once

#include "utils/Math.h"

#include <optional>

namespace engine::input
{
// Eigen types, zodat de game geen SDL_Event nodig heeft. Alleen wat de PoC gebruikt.
enum class EventType
{
    Quit,
    KeyDown,
    MouseMotion,
};

enum class Key
{
    Unknown,
    Num1,
    Num2,
    Up,
    Down,
    Space,
    A,
    Escape,
};

struct Event
{
    EventType type{EventType::Quit};
    Key key{Key::Unknown}; // alleen bij KeyDown
    utils::Vec2 mouse{};   // alleen bij MouseMotion
};

// Geeft het volgende event, of std::nullopt als de wachtrij leeg is.
std::optional<Event> pollEvent();
} // namespace engine::input
