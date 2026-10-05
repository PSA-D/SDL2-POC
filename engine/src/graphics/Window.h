#pragma once

#include <SDL.h>
#include <memory>
#include <string>

namespace engine::graphics
{
class Window
{
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;
    Window(Window&&) = delete;
    auto operator=(Window&&) -> Window& = delete;

    [[nodiscard]] SDL_Window* handle() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

private:
    struct SdlWindowDeleter
    {
        void operator()(SDL_Window* sdlWindow) const;
    };

    std::unique_ptr<SDL_Window, SdlWindowDeleter> window;
};
}


