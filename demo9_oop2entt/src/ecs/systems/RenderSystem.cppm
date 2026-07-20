// ecs/systems/RenderSystem.cppm
export module ecs:RenderSystem;
import entt;
import <SDL2/SDL.h>;
import :RenderComponents;
import :TextureRegistry;
import <algorithm>;

export void sys_render_sprites(entt::registry& reg, SDL_Renderer* renderer) {
    auto& tex_reg = reg.ctx<TextureRegistry>();
    
    // 1. 静态精灵
    auto view_static = reg.view<Transform2D, Sprite>(entt::exclude<Animation>);
    for (auto e : view_static) {
        auto& tf = view_static.get<Transform2D>(e);
        auto& sp = view_static.get<Sprite>(e);
        SDL_Texture* tex = tex_reg.get(sp.texture_id);
        if (!tex) continue;

        SDL_Rect src{0, 0, (int)sp.width, (int)sp.height};
        SDL_Rect dst{(int)tf.position.x, (int)tf.position.y, (int)sp.width, (int)sp.height};
        SDL_SetTextureAlphaMod(tex, sp.alpha);
        SDL_RenderCopyEx(renderer, tex, &src, &dst, tf.rotation, nullptr, sp.flip);
    }

    // 2. 动画精灵
    auto view_anim = reg.view<Transform2D, Sprite, Animation>();
    for (auto e : view_anim) {
        auto& tf = view_anim.get<Transform2D>(e);
        auto& sp = view_anim.get<Sprite>(e);
        auto& an = view_anim.get<Animation>(e);
        SDL_Texture* tex = tex_reg.get(sp.texture_id);
        if (!tex) continue;

        SDL_Rect src{
            (int)(an.frame_w * an.current_frame),
            (int)(an.frame_h * (an.current_row - 1)),
            (int)an.frame_w, (int)an.frame_h
        };
        SDL_Rect dst{(int)tf.position.x, (int)tf.position.y, (int)sp.width, (int)sp.height};
        SDL_SetTextureAlphaMod(tex, sp.alpha);
        SDL_RenderCopyEx(renderer, tex, &src, &dst, tf.rotation, nullptr, sp.flip);
    }
}

export void sys_render_health_bars(entt::registry& reg, SDL_Renderer* renderer) {
    auto& tex_reg = reg.ctx<TextureRegistry>();
    SDL_Texture* grad = tex_reg.get("HealthGradient");
    if (!grad) return;

    auto view = reg.view<Transform2D, HealthBar>();
    for (auto e : view) {
        auto& tf = view.get<Transform2D>(e);
        auto& hb = view.get<HealthBar>(e);
        if (!hb.visible || hb.max_hp <= 0) continue;

        int x = (int)tf.position.x;
        int y = (int)tf.position.y;
        float ratio = std::clamp((float)hb.current_hp / hb.max_hp, 0.0f, 1.0f);

        // 背景
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_Rect bg{x, y, (int)hb.width, (int)hb.height};
        SDL_RenderFillRect(renderer, &bg);

        // 渐变前景
        if (ratio > 0.0f) {
            SDL_Rect src{0, 0, (int)(256 * ratio), 1};
            SDL_Rect dst{x, y, (int)(hb.width * ratio), (int)hb.height};
            SDL_RenderCopy(renderer, grad, &src, &dst);
        }

        // 边框
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &bg);
    }
}
