// RenderContext.cpp
#include "core/RenderContext.hpp"
#include <stdexcept>

std::unique_ptr<RenderContext> RenderContext::create(SDL_Window* window, int index, Uint32 flags) {
    if (!window) {
        throw std::invalid_argument("Window must not be null");
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, index, flags);
    if (!renderer) {
        throw std::runtime_error(std::string("Failed to create renderer: ") + SDL_GetError());
    }
    return std::make_unique<RenderContext>(renderer);
}

RenderContext::RenderContext(SDL_Renderer* renderer) : renderer_(renderer, RendererDeleter{}) {}

RenderContext::RenderContext(RenderContext&&) noexcept = default;
RenderContext& RenderContext::operator=(RenderContext&&) noexcept = default;

void RenderContext::clear(Uint32 color) {
    Uint8 r = (color >> 16) & 0xFF;
    Uint8 g = (color >> 8) & 0xFF;
    Uint8 b = color & 0xFF;
    Uint8 a = (color >> 24) & 0xFF;
    SDL_SetRenderDrawColor(renderer_.get(), r, g, b, a);
    SDL_RenderClear(renderer_.get());
}

void RenderContext::present() {
    SDL_RenderPresent(renderer_.get());
}

void RenderContext::setDrawColor(Uint32 argb) {
    Uint8 a = (argb >> 24) & 0xFF;
    Uint8 r = (argb >> 16) & 0xFF;
    Uint8 g = (argb >> 8) & 0xFF;
    Uint8 b = argb & 0xFF;
    SDL_SetRenderDrawColor(renderer_.get(), r, g, b, a);
}

void RenderContext::fillRect(int x, int y, int w, int h) {
    SDL_Rect rect{x, y, w, h};
    SDL_RenderFillRect(renderer_.get(), &rect);
}

void RenderContext::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer_.get(), x1, y1, x2, y2);
}
