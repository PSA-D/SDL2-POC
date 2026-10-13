#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace engine::graphics
{
// Facade: geen SDL-types in deze header. De implementatie staat in graphics/<ENGINE_BACKEND>/Window.cpp.
class Window
{
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    void setTitle(const std::string& title);

    // Id waarmee de Renderer (zelfde backend) het onderliggende venster terugvindt.
    [[nodiscard]] std::uint32_t id() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
} // namespace engine::graphics
