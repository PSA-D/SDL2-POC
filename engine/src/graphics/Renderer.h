#pragma once

#include "graphics/Window.h"

#include <SDL.h>

#include <cstdint>
#include <memory>

namespace engine::graphics
{
struct Color
{
    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};
    std::uint8_t a{255};
};

class Renderer
{
public:
    explicit Renderer(const Window& window);
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    auto operator=(const Renderer&) -> Renderer& = delete;
    Renderer(Renderer&&) = delete;
    auto operator=(Renderer&&) -> Renderer& = delete;

    void clear(Color color) const;
    void present() const;
    [[nodiscard]] SDL_Renderer* handle() const;

private:
    struct SdlRendererDeleter
    {
        void operator()(SDL_Renderer* sdlRenderer) const;
    };

    std::unique_ptr<SDL_Renderer, SdlRendererDeleter> renderer;
};
} // namespace engine::graphics