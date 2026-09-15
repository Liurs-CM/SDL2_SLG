#pragma once
#include "comp/CompGroup.hpp"
#include <entt/entt.hpp>
#include <iostream>

class MovePlayerSystem
{
    public:
        void update(entt::registry& registry) {
            auto view = registry.view<WantsToMove, Position, InputState>();
            for (auto [entity, pos, input] : view.each()) {
                Position newPos{pos.x + input.x, pos.y + input.y};
                if (isValidMove(registry, newPos)) pos = newPos; 
                //std::cout << "pos at: " << pos.x << ", " << pos.y;
            }
            // ⚠️ 关键：处理完立即清除标签, 确保下帧无输入时不会残留触发
            registry.clear<WantsToMove>();
        }
    private:
        bool isValidMove(entt::registry& registry, Position newPos) {
            //x = std::max(0, std::min(x, MAP_WIDTH - 1));
            //y = std::max(0, std::min(y, MAP_HEIGHT - 1));
            auto& tlg = registry.ctx().get<TileLayerGrid>();
            auto view = registry.view<CollidableLayer, TileMap>();
            for (auto entity : view) {
                auto& map= registry.get<TileMap>(entity);
                if (newPos.x < 0 || newPos.x >= tlg.width_num || newPos.y < 0 || newPos.y >= tlg.height_num) {
                    std::cout << "move out...";
                    std::cout << "  x: " << newPos.x << " at 0~" << tlg.width_num-1;
                    std::cout << "; y: " << newPos.y << " at 0~" << tlg.height_num-1 << "\n";
                    return false;
                }
                uint32_t tileId = map.tileIDs[newPos.y * tlg.width_num + newPos.x];
                if ( tileId != 0 ) {
                //if ( tileId == 1 ) {
                    std::cout << "move tile collidable..." << newPos << "\n";
                    return false; 
                }
            }
            auto view_obj = registry.view<CollidableLayer, Position>();
            for (auto [ObjEntity, pos] : view_obj.each()) {
                if(pos == newPos) {
                    std::cout << "move object collidable..." << newPos << "\n";
                    return false; 
                }
            }
            return true;
        }
};

