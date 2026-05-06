// Systems.hpp
#pragma once
#include "Components.hpp"
#include "TextureManager.h"
#include "Camera.h"
#include <entt/entt.hpp>

// 更新系统：替代原 update() 函数
void UpdateSystem(entt::registry& registry) {
    // 获取所有同时拥有 Transform 和 Render 组件的实体
    auto view = registry.view<TransformComponent, RenderComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& render = view.get<RenderComponent>(entity);
        // 物理/移动逻辑
        transform.velocity += transform.acceleration;
        transform.position += transform.velocity;
        // 摄像机逻辑
        render.screenPosition = transform.position - TheCamera::Instance()->getPosition();
        // 简单的动画帧更新逻辑 (模拟原 m_currentAnim.getCurrentFrame)
        render.currentFrame = (render.currentFrame + 1) % render.numFrames; 
    }
}

// 渲染系统：替代原 draw() 函数
void RenderSystem(entt::registry& registry) {
    auto view = registry.view<TransformComponent, RenderComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& render = view.get<RenderComponent>(entity);
        // 绘图逻辑
        if (transform.velocity.getX() >= 0) {
            TextureManager::Instance()->drawFrame(
                    render.textureID, 
                    render.screenPosition, 
                    render.width, 
                    render.height, 
                    render.currentRow, 
                    render.currentFrame
                    );
        } else {
            TextureManager::Instance()->drawFrame(
                    render.textureID, 
                    render.screenPosition, 
                    render.width, 
                    render.height, 
                    render.currentRow, 
                    render.currentFrame, 
                    255, 
                    SDL_FLIP_HORIZONTAL
                    );
        }
    }
}

void AnimatedGraphicSystem(entt::registry& registry)
{
    static inline uint8_t globalFrame = 0;
    globalFrame = (globalFrame + 1) % FPS;
    auto view = registry.view<AnimationComponent>();
    for (auto entity : view) {
        auto& animComp = view.get<AnimationComponent>(entity);
        animComp.currentFrame = (animComp.animSpeed * globalFrame / FPS) % animComp.numFrames;
    }
}
