#pragma once
#include "Pos.hpp"
#include "Sprites.hpp"
#include <SDL_render.h>

namespace SDL {

    class QuadWriter {
        public:
            QuadWriter(SDL_Renderer *, SDL_Texture *);
            void tilePos(Pos, Pos, double = 0.0);
            void tileTex(animera::SpriteRect);
            void tileTex(animera::SpriteID);
            void render() const;

        private:
            SDL_Renderer *renderer;
            SDL_Texture *texture;
            SDL_Rect srcRect;
            SDL_Rect dstRect;
            double angle;
    };

}
