module;
#include <SDL2/SDL.h>
export module core.render_context;

export namespace core {
    class RenderContext {
        public:
            bool init(const char* title, int w, int h) {
                if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
                window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
                if (!window_) return false;
                renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                return renderer_ != nullptr;
            }
            void clear()  { SDL_RenderClear(renderer_); }
            void present(){ SDL_RenderPresent(renderer_); }
            void shutdown(){
                if (renderer_) SDL_DestroyRenderer(renderer_);
                if (window_)   SDL_DestroyWindow(window_);
                SDL_Quit();
            }
            SDL_Renderer* get() const { return renderer_; }
        private:
            SDL_Window*   window_   = nullptr;
            SDL_Renderer* renderer_ = nullptr;
    };
}
