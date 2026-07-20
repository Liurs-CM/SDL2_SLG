module;
#include <SDL2/SDL.h>
export module core.RenderUtils;

import utils.Vector2D;
import <string_view>;
import <cstdint>;

export namespace render {
    // 替代原 drawFrame / drawRotat
    inline void draw_texture(SDL_Renderer* renderer, SDL_Texture* tex, 
            vec2 pos, uint16_t w, uint16_t h, 
            double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE,
            uint8_t alpha = 255) {
        if (!renderer || !tex) return;
        SDL_SetTextureAlphaMod(tex, alpha);
        SDL_Rect dst{static_cast<int>(pos.x), static_cast<int>(pos.y), 
            static_cast<int>(w), static_cast<int>(h)};
        SDL_RenderCopyEx(renderer, tex, nullptr, &dst, angle, nullptr, flip);
    }

    // 替代原 drawHealthBar（纯数据驱动，不持有状态）
    inline void draw_health_bar(SDL_Renderer* renderer, vec2 pos, 
            uint16_t w, uint16_t h, 
            float hp_ratio, uint32_t bg_color = 0x333333FF, 
            uint32_t fg_color = 0x00FF00FF) {
        if (!renderer || hp_ratio < 0.0f) return;
        hp_ratio = (hp_ratio > 1.0f) ? 1.0f : hp_ratio;

        // 背景
        SDL_SetRenderDrawColor(renderer, (bg_color>>24)&0xFF, (bg_color>>16)&0xFF, 
                (bg_color>>8)&0xFF, bg_color&0xFF);
        SDL_Rect bg{static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(w), static_cast<int>(h)};
        SDL_RenderFillRect(renderer, &bg);

        // 前景
        SDL_SetRenderDrawColor(renderer, (fg_color>>24)&0xFF, (fg_color>>16)&0xFF, 
                (fg_color>>8)&0xFF, fg_color&0xFF);
        SDL_Rect fg{static_cast<int>(pos.x), static_cast<int>(pos.y), 
            static_cast<int>(w * hp_ratio), static_cast<int>(h)};
        SDL_RenderFillRect(renderer, &fg);
    }
}
