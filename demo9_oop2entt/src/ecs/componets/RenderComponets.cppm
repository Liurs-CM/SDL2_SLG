// ecs/components/RenderComponents.cppm
export module ecs.RenderComponents;
import utils.Vector2D; // 假设已定义 vec2
import <SDL2/SDL.h>;
import <string>;
import <cstdint>;

export struct Transform2D {
    vec2 position{0.0f, 0.0f};
    float rotation = 0.0f;
    vec2 scale{1.0f, 1.0f};
};

export struct Sprite {
    std::string texture_id;
    uint16_t width  = 0;
    uint16_t height = 0;
    int alpha       = 255;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
};

export struct Animation {
    uint8_t current_frame = 0;
    uint8_t current_row   = 1;
    uint16_t frame_w      = 0;
    uint16_t frame_h      = 0;
};

export struct HealthBar {
    int current_hp = 0;
    int max_hp     = 100;
    uint16_t width = 50;
    uint16_t height= 6;
    bool visible   = true;
};

export struct RenderLayer {
    int z_order = 0; // 用于渲染排序
};
