#pragma once

#include "graphics/Color.h"
#include "graphics/Window.h"
#include "utils/Math.h"

#include <memory>

namespace engine::graphics
{
// Facade: geen SDL-types in deze header. De implementatie staat in graphics/<ENGINE_BACKEND>/Renderer.cpp.
class Renderer
{
public:
    explicit Renderer(const Window& window);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void clear(Color color);
    void fillRect(utils::Vec2 position, utils::Vec2 size, Color color);
    void present();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
} // namespace engine::graphics
