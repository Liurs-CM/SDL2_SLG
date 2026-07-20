#pragma once
#include "core/InputHandler.h"
#include "comp/CompGroup.hpp"
#include "sys/TouchSystem.hpp"
#include "util/Vector2D.h"
#include <entt/entt.hpp>

class InputSystem {
    public:
        void update(entt::registry& registry) {
            auto view = registry.view<PlayerTag, InputState>();
            // 获取单例实例 (注意：这是唯一允许接触 InputHandler 的地方)
            auto* handler = InputHandler::Instance();
            for (auto entity : view) {
                auto& input = registry.get<InputState>(entity);
                auto& pos = registry.get<Position>(entity);
                // 重置当前帧输入
                input.dx = 0;
                input.dy = 0;
                // 检查按键并累加方向
                if(handler->isKeyPressed(SDL_SCANCODE_W) || handler->isKeyPressed(SDL_SCANCODE_UP)) {
                    input.dy -= 1;
                    registry.emplace_or_replace<Direction>(entity, Direction::UP);
                }
                if(handler->isKeyPressed(SDL_SCANCODE_S) || handler->isKeyPressed(SDL_SCANCODE_DOWN)) {
                    input.dy += 1;
                    registry.emplace_or_replace<Direction>(entity, Direction::DOWN);
                }
                if(handler->isKeyPressed(SDL_SCANCODE_A) || handler->isKeyPressed(SDL_SCANCODE_LEFT)) {
                    input.dx -= 1;
                    registry.emplace_or_replace<Direction>(entity, Direction::LEFT);
                }
                if(handler->isKeyPressed(SDL_SCANCODE_D) || handler->isKeyPressed(SDL_SCANCODE_RIGHT)) {
                    input.dx += 1;
                    registry.emplace_or_replace<Direction>(entity, Direction::RIGHT);
                }
                if(handler->isKeyPressed(SDL_SCANCODE_SPACE)) {
                    //registry.emplace_or_replace<Direction>(entity, Direction::RIGHT);
                    auto& dir = registry.get<Direction>(entity);
                    vec dv = Dir2Vec(dir);
                    UIMsgSystem::Instance().setInfoMsg("[SYS] touch something...at {%d, %d} to {%d, %d}", pos.x, pos.y, dv.x, dv.y);
                    TouchSystem::touch(registry, entity, pos + dv, UIMsgSystem::Instance());
                }
                // 也可以在这里处理手柄输入...
            }
        }
};
