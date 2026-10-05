#include "Window.h"

#include "graphics/Window.h"

#include <stdexcept>
#include <string>

namespace engine::graphics
{
void Window::SdlWindowDeleter::operator()(SDL_Window* sdlWindow) const
{
    SDL_DestroyWindow(sdlWindow);
}

Window::Window(const std::string& title, int width, int height)
{
    // Nodig omdat we SDL_MAIN_HANDLED gebruiken (geen SDL2main).
    SDL_SetMainReady();

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(std::string("SDL_InitSubSystem failed: ") + SDL_GetError());

    window.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, SDL_WINDOW_SHOWN));
    if (!window)
    {
        // Een destructor draait niet als de constructor gooit, dus hier zelf opruimen.
        const std::string error = SDL_GetError();
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        throw std::runtime_error("SDL_CreateWindow failed: " + error);
    }
}

Window::~Window()
{
    window.reset();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

SDL_Window* Window::handle() const
{
    return window.get();
}

int Window::width() const
{
    int w{0};
    int h{0};
    SDL_GetWindowSize(window.get(), &w, &h);
    return w;
}

int Window::height() const
{
    int w{0};
    int h{0};
    SDL_GetWindowSize(window.get(), &w, &h);
    return h;
}
} // namespace engine::graphics