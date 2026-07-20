#pragma once
#include "core/GameConfig.h"
#include "comp/CompGroup.hpp"
#include "FastNoiseLite.h"
#include <entt/entt.hpp>
#include <vector>

// 地形类型
enum GID {
    DEEP_WATER = 0,
    WATER = 1,
    SAND = 2,
    DIRT = 3,
    GRASS = 4,
    SNOW = 5,
};

class MapGeneratorSystem {
    public:
        std::vector<float> generateNoiseMap(int w, int h, unsigned seed) {
            FastNoiseLite noise;
            noise.SetSeed(seed);
            noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
            noise.SetFractalType(FastNoiseLite::FractalType_FBm);
            noise.SetFractalOctaves(4);
            noise.SetFractalLacunarity(2.0f);
            noise.SetFractalGain(0.5f);
            noise.SetFrequency(0.04f);
            std::vector<float> map(w * h);
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    // FastNoiseLite 直接返回 [-1, 1]
                    float val = noise.GetNoise(static_cast<float>(x), static_cast<float>(y));
                    map[y * w + x] = (val + 1.0f) * 0.5f;  // 映射到 [0, 1]
                }
            }
            return map;
        }

        void generate(entt::registry& reg, unsigned seed) {
            // 1. 填充 TileLayerGrid
            // 2. 生成噪声高度图
            auto MAP_HEIGHTMap = generateNoiseMap(MAP_WIDTH, MAP_HEIGHT, seed);
            // 3. 创建 TileMap entity
            auto entity = reg.create();
            auto& tileMap = reg.emplace<TileMap>(entity);
            tileMap.tileIDs.resize(MAP_WIDTH * MAP_HEIGHT);
            // 4. 映射为 GID
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                for (int x = 0; x < MAP_WIDTH; ++x) {
                    float noise = MAP_HEIGHTMap[y * MAP_WIDTH + x];
                    tileMap.tileIDs[y * MAP_WIDTH + x] = noiseToGID(noise);
                }
            }
        }

    private:
        // 噪声值 → 地形 GID（需与 tileset 对齐）
        uint32_t noiseToGID(float noise) {
            if (noise < 0.4f) return GID::WATER;
            if (noise < 0.45f) return GID::SAND;
            if (noise < 0.65f) return GID::DIRT;
            if (noise < 0.75f) return GID::GRASS;
            return GID::SNOW;
        }
};

