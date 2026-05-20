#pragma once
#include "SDL2/SDL_render.h"

class RenderContext {
    public:
        // 绑定当前渲染器（通常在主循环开始处调用一次）
        static void set(SDL_Renderer* renderer) {
            s_renderer = renderer;
        }

        // 获取当前渲染器（供 TextureManager 等内部使用）
        static SDL_Renderer* get() {
            return s_renderer;
        }

    private:
        static SDL_Renderer* s_renderer; // 全局唯一，单线程安全
};

