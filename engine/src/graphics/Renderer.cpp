#include "graphics/Renderer.h"

#include <stdexcept>
#include <string>

namespace engine::graphics
{
namespace
{
constexpr Uint32 RENDERER_FLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
}

void Renderer::SdlRendererDeleter::operator()(SDL_Renderer* sdlRenderer) const
{
    SDL_DestroyRenderer(sdlRenderer);
}

Renderer::Renderer(const Window& window)
    : renderer(SDL_CreateRenderer(window.handle(), -1, RENDERER_FLAGS))
{
    if (!renderer)
        throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
}

void Renderer::clear(Color color) const
{
    SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer.get());
}

void Renderer::present() const
{
    SDL_RenderPresent(renderer.get());
}

SDL_Renderer* Renderer::handle() const
{
    return renderer.get();
}
} // namespace engine::graphics