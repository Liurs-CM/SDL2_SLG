#pragma once
#include "core/GameConfig.h"
#include "core/RenderContext.hpp"
#include "comp/CompGroup.hpp"
#include <entt/entt.hpp>
#include <SDL.h>
#include <iostream>

class CameraSystem {
    public:
        void update(entt::registry& registry) {
            auto& tlg = registry.ctx().get<TileLayerGrid>();
            auto mapView = registry.view<TileMap>();
            if (mapView.empty()) { std::cout << "not map for camera...\n"; return;} // 没有地图就不更新摄像机
            auto view = registry.view<PlayerTag, Position>();
            for (auto entity : view) {
                const auto& pos = registry.get<Position>(entity);
                // 2. 计算摄像机中心点
                // 假设 TileSize 是固定的，或者从配置中获取
                int targetGridX = static_cast<int>(pos.x - CELL_X_NUM / 2.0f);
                int targetGridY = static_cast<int>(pos.y - CELL_Y_NUM / 2.0f);
                int maxGridX = tlg.width_num - CELL_X_NUM;
                int maxGridY = tlg.height_num - CELL_Y_NUM;
                // 3. 添加边界限制，防止摄像机移出地图
                targetGridX = std::max(0, std::min(targetGridX, maxGridX));
                targetGridY = std::max(0, std::min(targetGridY, maxGridY));
                // 4. 应用摄像机变换
                RenderContext::setGridOffset({targetGridX, targetGridY});
            }
        }
    private:
};
