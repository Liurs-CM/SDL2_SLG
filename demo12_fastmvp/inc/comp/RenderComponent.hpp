#pragma once
#include "util/Vector2D.h"
#include <SDL2/SDL_render.h>
#include <string>

// 纹理资源句柄（轻量级引用）
struct Sprite {
    std::string textureId;  // 纹理资源 ID
    SDL_Rect srcRect;       // 源矩形（用于精灵表）
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int alpha = 255;
    double angle = 0.0;     // 旋转角度
    SDL_Point center;       // 旋转中心
};

// 位置和变换
struct Transform {
    vec2 pos;
    vec2 scale = {1.0f, 1.0f};
    vec size = {0, 0};
};

// 渲染组件
//struct Renderable {
//    AtlasImage *texture;
//    int facing; // FACING_LEFT or FACING_RIGHT
//    bool center; // 是否居中绘制
//};

// 实体标签组件
struct EntityTag {
    const char* name;
};

// 生命值组件
struct Health {
    int current = 100;
    int max = 100;
};

// 调试绘制组件
struct DebugDraw {
    enum Type { LINE, CIRCLE, GRID, TEXT };
    Type type;
    // ... 其他参数
};


