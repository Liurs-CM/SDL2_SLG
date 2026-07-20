#pragma once
#include "core/GameConfig.h"
#include "core/RenderContext.hpp"
#include "core/TextureManager.hpp"
#include "comp/CompGroup.hpp"
#include "sys/UIMsgSystem.hpp"
#include "entt/entt.hpp"
#include <SDL2/SDL_render.h>
#include <iostream>

// sys/RenderSystem.hpp
class RenderSystem {
    public:
        RenderSystem() {}
        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;
        void setRenderer(SDL_Renderer* renderer, entt::registry* registry) { 
            m_renderer = renderer; 
            m_registry = registry;
            m_viewportChanged = true; // 初始化时强制更新
        }
        void update(){
            if (!m_renderer) {
                std::cout << "no renderer...\n";
                return; // ✅ 安全检查
            }
            checkViewportChange();
            renderTileMap();
            renderObjs();
            renderSprites();
            renderHud();
        };
        void renderTileMap() {
            const auto& tlg = m_registry->ctx().get<TileLayerGrid>();
            // ====== 1. 视锥剔除：仅计算可见范围 ======
            const int grid_offsetX = RenderContext::getGridOffset().x;
            const int grid_offsetY = RenderContext::getGridOffset().y;
            const int startCol = std::max(0, grid_offsetX - 1);
            const int startRow = std::max(0, grid_offsetY - 1);
            const int endCol   = std::min(tlg.width_num, grid_offsetX + CELL_X_NUM + 1);
            const int endRow   = std::min(tlg.height_num, grid_offsetY + CELL_Y_NUM + 1);
            // 只在视窗移动时重建顶点缓存
            if (m_viewportChanged) {
                rebuildTileCache(startCol, startRow, endCol, endRow, grid_offsetX, grid_offsetY);
                m_viewportChanged = false;
            }
            // 渲染缓存的批次
            for (const auto& batch : m_cachedBatches) {
                if (!batch.vertices.empty()) {
                    SDL_RenderGeometry(m_renderer, batch.texture,
                            batch.vertices.data(), batch.vertices.size(),
                            batch.indices.data(), batch.indices.size());
                }
            }
        }

        void rebuildTileCache(int startCol, int startRow, int endCol, int endRow, int grid_offsetX, int grid_offsetY) {
            m_cachedBatches.clear();
            auto view_tc = m_registry->view<const TilesetComponent>();
            for (auto [tilesetEntity, tc] : view_tc.each()) {
                SDL_Texture* tex = TheTextureManager::Instance()->getTexture(tc.name);
                if (!tex) { std::cout << tc.name << " texture not exist...\n"; continue; }
                CachedTileBatch batch;
                batch.texture = tex;
                for (auto& tsRange : tc.tsRanges) {
                    for (auto& info : tsRange.tileInfos) {
                        // 直接使用预缓存的坐标 - 零计算开销视锥剔除（此时数据局部性极好，cache friendly）
                        if (info.GridX < startCol || info.GridX >= endCol || info.GridY < startRow || info.GridY >= endRow) continue;
                        // 4. 打包顶点（4 vertices + 6 indices per tile）
                        const float su = static_cast<float>(info.srcX) / tc.imageWidth;
                        const float sv = static_cast<float>(info.srcY) / tc.imageHeight;
                        const float eu = su + static_cast<float>(tc.tileWidth)  / tc.imageWidth;
                        const float ev = sv + static_cast<float>(tc.tileHeight) / tc.imageHeight;
                        const float dx = static_cast<float>(info.dstX - grid_offsetX * CELL_SIZE);
                        const float dy = static_cast<float>(info.dstY - grid_offsetY * CELL_SIZE);
                        const float dw = static_cast<float>(CELL_SIZE);
                        const float dh = static_cast<float>(CELL_SIZE);
                        const SDL_Color white{255, 255, 255, 255};
                        const size_t base = batch.vertices.size();
                        // 添加4个顶点
                        batch.vertices.push_back({{dx,      dy     }, white, {su, sv}});
                        batch.vertices.push_back({{dx + dw, dy     }, white, {eu, sv}});
                        batch.vertices.push_back({{dx + dw, dy + dh}, white, {eu, ev}});
                        batch.vertices.push_back({{dx,      dy + dh}, white, {su, ev}});
                        // 添加6个索引（组成2个三角形）
                        batch.indices.push_back(base + 0);
                        batch.indices.push_back(base + 1);
                        batch.indices.push_back(base + 2);
                        batch.indices.push_back(base + 0);
                        batch.indices.push_back(base + 2);
                        batch.indices.push_back(base + 3);
                    }
                }
                // 5. ✅ 单次 DrawCall 提交整个纹理批次
                // 只添加非空的批次
                if (!batch.vertices.empty()) {
                    m_cachedBatches.push_back(std::move(batch));
                }
            }
        }

        // 渲染所有精灵
        void renderSprites() {
            const int grid_offsetX = RenderContext::getGridOffset().x;
            const int grid_offsetY = RenderContext::getGridOffset().y;
            auto view = m_registry->view<const SpriteComponent, const Position>();
            //std::cout << view.size() << "\n";
            for (auto [entity, sprite, Pos] : view.each()) {
                SDL_Texture* tex = TheTextureManager::Instance()->getTexture(sprite.textureID);
                if (!tex) { std::cout << sprite.textureID << " texture not exist...\n"; continue; }
                SDL_Rect srcRect = {
                    static_cast<int>(0),
                    static_cast<int>(sprite.height * sprite.curFrame),
                    static_cast<uint8_t>(sprite.width),
                    static_cast<uint8_t>(sprite.height)
                };
                SDL_Rect dstRect = {
                    static_cast<int>((Pos.x - grid_offsetX) * CELL_SIZE),
                    static_cast<int>((Pos.y - grid_offsetY) * CELL_SIZE),
                    static_cast<uint8_t>(sprite.width),
                    static_cast<uint8_t>(sprite.height)
                };
                SDL_RenderCopy(m_renderer, tex, &srcRect, &dstRect);
                //SDL_RenderCopyEx(m_renderer, texture, 
                //        &srcRect, &destRect,
                //        sprite.angle, &sprite.center, sprite.flip);
                // 处理旋转
                //if (sprite.angle != 0.0) {
                //} else {
                //    SDL_RenderCopyEx(m_renderer, texture,
                //            &sprite.srcRect, &destRect,
                //            0, nullptr, sprite.flip);
                //}
            }
        }

        void renderObjs() {
            const int grid_offsetX = RenderContext::getGridOffset().x;
            const int grid_offsetY = RenderContext::getGridOffset().y;
            auto view_tc = m_registry->view<TilesetComponent>();
            for (auto [tilesetEntity, tc] : view_tc.each()) { 
                if (tc.name != "blocks_item") { continue; }
                auto view = m_registry->view<const ObjectComponent, const Position>();
                SDL_Texture* tex = TheTextureManager::Instance()->getTexture(tc.name);
                if (!tex) { std::cout << tc.name << " texture not exist...\n"; continue; }
                //std::cout << view.size() << "\n";
                for (auto [entity, obj, Pos] : view.each()) {
                    if (obj.tilename != tc.name) { continue; }
                    uint32_t tileID = obj.firstTileID + obj.curFrame - tc.firstGid;
                    //std::cout << "render obj:" << tc.name << " " << tileID << " " << (tileID % tc.numColumns) << " " << (tileID / tc.numColumns) << "\n";
                    SDL_Rect srcRect = {
                        static_cast<int>(tc.margin + (tileID % tc.numColumns) * (tc.tileWidth + tc.spacing)),
                        static_cast<int>(tc.margin + (tileID / tc.numColumns) * (tc.tileHeight + tc.spacing)),
                        static_cast<uint8_t>(tc.tileWidth),
                        static_cast<uint8_t>(tc.tileHeight)
                    };
                    SDL_Rect dstRect = {
                        static_cast<int>((Pos.x - grid_offsetX) * CELL_SIZE),
                        static_cast<int>((Pos.y - grid_offsetY) * CELL_SIZE),
                        static_cast<uint8_t>(tc.tileWidth),
                        static_cast<uint8_t>(tc.tileHeight)
                    };
                    SDL_RenderCopy(m_renderer, tex, &srcRect, &dstRect);
                }
            }
        }

        void renderHud() {
            UIMsgSystem::Instance().render();
            UIMsgSystem::Instance().doneMsg();
        }
        // 渲染生命条
        void renderHealthBars() {
            //auto view = m_registry.view<const Health, const Transform>();
            // ... 绘制逻辑
        }

        // 可选：手动标记需要更新缓存（当tilemap数据改变时调用）
        void invalidateCache() {
            m_viewportChanged = true;
        }

        // 可选：缓存纹理本身（如果需要）
        struct CachedTexture {
            SDL_Texture* texture = nullptr;
            SDL_Rect sourceRect;
            SDL_Rect destRect;
        };

        std::vector<CachedTexture> m_cachedTextures;  // 如果选择缓存纹理方案
    private:
        SDL_Renderer* m_renderer;
        entt::registry* m_registry;
        // 视窗状态追踪
        int m_lastGridX = -1;
        int m_lastGridY = -1;
        bool m_viewportChanged = true;
        // 缓存结构：为每个纹理存储预计算的顶点和索引
        struct CachedTileBatch {
            std::vector<SDL_Vertex> vertices;
            std::vector<int> indices;
            SDL_Texture* texture = nullptr;
        };
        // 顶点缓存
        std::vector<CachedTileBatch> m_cachedBatches;
        void checkViewportChange() {
            const int currentGridX = RenderContext::getGridOffset().x;
            const int currentGridY = RenderContext::getGridOffset().y;

            if (currentGridX != m_lastGridX || currentGridY != m_lastGridY) {
                m_viewportChanged = true;
                m_lastGridX = currentGridX;
                m_lastGridY = currentGridY;
            }
        }
};
