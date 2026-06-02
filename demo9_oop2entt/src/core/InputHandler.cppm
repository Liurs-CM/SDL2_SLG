module;
#include <SDL2/SDL.h>
export module core.input_handler;

export namespace core {
    class InputHandler {
        public:
            void update() {
                SDL_Event e;
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) should_quit_ = true;
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) should_quit_ = true;
                }
            }
            bool should_quit() const { return should_quit_; }
        private:
            bool should_quit_ = false;
    };
}
