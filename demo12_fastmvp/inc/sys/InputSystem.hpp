#pragma once
#include "core/InputHandler.h"
#include "comp/CompGroup.hpp"
#include "sys/TouchSystem.hpp"
#include "util/Vector2D.h"
#include <entt/entt.hpp>

class InputSystem {
    public:
        struct MoveMapping {
            SDL_Scancode key;
            vec dir;
            //Direction dir;
            //int dx, dy;
        };
        //static const MoveMapping MOVE_MAP[];
        inline static const MoveMapping MOVE_MAP[] = {
            { SDL_SCANCODE_W,     vec::up    },
            { SDL_SCANCODE_UP,    vec::up    },
            { SDL_SCANCODE_S,     vec::down  },
            { SDL_SCANCODE_DOWN,  vec::down  },
            { SDL_SCANCODE_A,     vec::left  },
            { SDL_SCANCODE_LEFT,  vec::left  },
            { SDL_SCANCODE_D,     vec::right },
            { SDL_SCANCODE_RIGHT, vec::right },
        };
        void update(entt::registry& registry) {
            timeLapse = false;
            // 获取单例实例 (注意：这是唯一允许接触 InputHandler 的地方)
            auto* handler = InputHandler::Instance();
            auto entity  = registry.ctx().get<PlayerRef>().id;
            auto& input = registry.get<InputState>(entity);
            auto& pos = registry.get<Position>(entity);
            // 重置当前帧输入
            input = {0, 0};
            // 检查按键并累加方向
            for (const auto& m : MOVE_MAP) {
                if (handler->isKeyPressed(m.key)) {
                    input += m.dir;
                    registry.emplace_or_replace<Direction>(entity, m.dir.getVecDir());
                }
            }
            // ✅ 核心：有输入才打标签，无输入则移除标签
            if (input.x != 0 || input.y != 0) {
                registry.emplace<WantsToMove>(entity);
                timeLapse = true;
            } else {
                registry.remove<WantsToMove>(entity);
            }
            if(handler->isKeyPressed(SDL_SCANCODE_SPACE)) {
                if (auto* dir = registry.try_get<Direction>(entity); dir) {
                    vec dv = Dir2Vec(*dir);
                    UIMsgSystem::Instance().setInfoMsg("[SYS] touch something...at {%d, %d} to {%d, %d}", pos.x, pos.y, dv.x, dv.y);
                    TouchSystem::touch(registry, entity, pos + dv, UIMsgSystem::Instance());
                    timeLapse = true;
                }
            }
            if(handler->isKeyPressed(SDL_SCANCODE_RETURN)) {
                inventorySys().dropItem(registry, entity);
                timeLapse = true;
            }
            if(handler->isKeyPressed(SDL_SCANCODE_TAB)) {
                inventorySys().cycleSlot(registry, entity, 1);
            }
            // 也可以在这里处理手柄输入...
        }
        bool Acted(){ return timeLapse; }
    private:
        bool timeLapse = false;
};

