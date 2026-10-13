#include "input/Input.h"

#include <SDL.h>

namespace engine::input
{
namespace
{
Key toKey(SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_1:
        return Key::Num1;
    case SDLK_2:
        return Key::Num2;
    case SDLK_UP:
        return Key::Up;
    case SDLK_DOWN:
        return Key::Down;
    case SDLK_SPACE:
        return Key::Space;
    case SDLK_a:
        return Key::A;
    case SDLK_ESCAPE:
        return Key::Escape;
    default:
        return Key::Unknown;
    }
}
} // namespace

std::optional<Event> pollEvent()
{
    SDL_Event event{};
    while (SDL_PollEvent(&event) != 0)
    {
        // SDL-events die we niet vertalen slaan we over, zodat de lus van de game niet te vroeg stopt.
        switch (event.type)
        {
        case SDL_QUIT:
            return Event{.type = EventType::Quit};
        case SDL_KEYDOWN:
            if (event.key.repeat == 0)
                return Event{.type = EventType::KeyDown, .key = toKey(event.key.keysym.sym)};
            break;
        case SDL_MOUSEMOTION:
            return Event{.type = EventType::MouseMotion,
                         .mouse = {static_cast<float>(event.motion.x), static_cast<float>(event.motion.y)}};
        default:
            break;
        }
    }
    return std::nullopt;
}
} // namespace engine::input
