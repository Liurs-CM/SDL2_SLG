#pragma once
#include "Sprites.hpp"
#include "SDL_Delete.hpp"

namespace SDL {
    Texture loadTexture(SDL_Renderer *, const animera::TextureInfo &);
}
