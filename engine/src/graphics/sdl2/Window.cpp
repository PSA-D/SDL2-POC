#include "graphics/Window.h"

#include <SDL.h>
#include <stdexcept>
#include <string>

namespace engine::graphics
{
struct Window::Impl
{
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, &SDL_DestroyWindow};
};

Window::Window(const std::string& title, int width, int height)
    : impl(std::make_unique<Impl>())
{
    SDL_SetMainReady(); // nodig omdat we SDL_MAIN_HANDLED gebruiken (geen SDL2main)
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(std::string("SDL_InitSubSystem(VIDEO) mislukt: ") + SDL_GetError());

    impl->window.reset(SDL_CreateWindow(
        title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));
    if (!impl->window)
    {
        const std::string error = SDL_GetError();
        SDL_QuitSubSystem(SDL_INIT_VIDEO); // de destructor draait niet als de constructor gooit
        throw std::runtime_error("SDL_CreateWindow mislukt: " + error);
    }
}

Window::~Window()
{
    impl->window.reset();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Window::setTitle(const std::string& title)
{
    SDL_SetWindowTitle(impl->window.get(), title.c_str());
}

std::uint32_t Window::id() const
{
    return SDL_GetWindowID(impl->window.get());
}
} // namespace engine::graphics
