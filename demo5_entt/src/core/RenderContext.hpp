#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>

class RenderContext {
    public:
        RenderContext(const RenderContext&) = delete;
        RenderContext& operator=(const RenderContext&) = delete;
        RenderContext(RenderContext&&) noexcept;
        RenderContext& operator=(RenderContext&&) noexcept;
        // Factory function: create rendering context (recommended approach)
        static std::unique_ptr<RenderContext> create(
                SDL_Window* window,
                int index = -1,
                Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                );
        // High-level renderer access (example)
        void clear(Uint32 color = 0xFF000000);
        void present();
        /// @param argb Color in RGBA format (e.g., 0xFF0000FF for red)
        void setDrawColor(Uint32 argb);
        void fillRect(int x, int y, int w, int h);
        void drawLine(int x1, int y1, int x2, int y2);
        // Low-level renderer access (use with caution, internal systems only)
        SDL_Renderer* getRaw() const { return renderer_.get(); }

    private:
        struct RendererDeleter {
            void operator()(SDL_Renderer* r) const {
                if (r) SDL_DestroyRenderer(r);
            }
        };
        explicit RenderContext(SDL_Renderer* renderer);
        std::unique_ptr<SDL_Renderer, RendererDeleter> renderer_;
};
