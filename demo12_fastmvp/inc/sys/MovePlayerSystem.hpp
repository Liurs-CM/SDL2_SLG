#pragma once
#include "comp/CompGroup.hpp"
#include <entt/entt.hpp>
#include <iostream>

class MovePlayerSystem
{
    public:
        void update(entt::registry& registry) {
            auto view = registry.view<PlayerTag, Position, InputState>();
            for (auto playerEntity : view) {
                auto& pos = registry.get<Position>(playerEntity);
                auto& input = registry.get<InputState>(playerEntity);
                int newX = pos.x + input.dx;
                int newY = pos.y + input.dy;
                if (isValidMove(registry, newX, newY)) { 
                    pos = {newX, newY}; 
                    //std::cout << "pos at: " << pos.x << ", " << pos.y;
                }
            }
        }
    private:
        bool isValidMove(entt::registry& registry, int x, int y) {
            //x = std::max(0, std::min(x, MAP_WIDTH - 1));
            //y = std::max(0, std::min(y, MAP_HEIGHT - 1));
            auto& tlg = registry.ctx().get<TileLayerGrid>();
            auto view = registry.view<CollidableLayer, TileMap>();
            for (auto entity : view) {
                auto& map= registry.get<TileMap>(entity);
                if (x < 0 || x >= tlg.width_num || y < 0 || y >= tlg.height_num) {
                    std::cout << "move out...";
                    std::cout << "  x: " << x << " at 0~" << tlg.width_num-1;
                    std::cout << "; y: " << y << " at 0~" << tlg.height_num-1 << "\n";
                    return false;
                }
                uint32_t tileId = map.tileIDs[y * tlg.width_num + x];
                if ( tileId != 0 ) {
                //if ( tileId == 1 ) {
                    std::cout << "move collidable...\n";
                    return false; 
                }
            }
            return true;
        }
};

