#include "graphics/Renderer.h"

#include <SDL.h>
#include <stdexcept>
#include <string>

namespace engine::graphics
{
struct Renderer::Impl
{
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{nullptr, &SDL_DestroyRenderer};
};

Renderer::Renderer(const Window& window)
    : impl(std::make_unique<Impl>())
{
    impl->renderer.reset(SDL_CreateRenderer(SDL_GetWindowFromID(window.id()), -1, SDL_RENDERER_PRESENTVSYNC));
    if (!impl->renderer)
        throw std::runtime_error(std::string("SDL_CreateRenderer mislukt: ") + SDL_GetError());

    // Alpha-blending aan, zodat particles kunnen vervagen via de alpha van hun kleur.
    SDL_SetRenderDrawBlendMode(impl->renderer.get(), SDL_BLENDMODE_BLEND);
}

Renderer::~Renderer() = default;

void Renderer::clear(Color color)
{
    SDL_SetRenderDrawColor(impl->renderer.get(), color.r, color.g, color.b, color.a);
    SDL_RenderClear(impl->renderer.get());
}

void Renderer::fillRect(utils::Vec2 position, utils::Vec2 size, Color color)
{
    const SDL_FRect rect{position.x, position.y, size.x, size.y};
    SDL_SetRenderDrawColor(impl->renderer.get(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(impl->renderer.get(), &rect);
}

void Renderer::present()
{
    SDL_RenderPresent(impl->renderer.get());
}
} // namespace engine::graphics
