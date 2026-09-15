#pragma once
#include "comp/CompGroup.hpp"
#include "comp/PlantComponet.hpp"
#include <entt/entt.hpp>

class GrowthSystem
{
    public:
        // 每个行动点调用一次
        void update(entt::registry& reg, const GrowthTiming t = GrowthTiming{}) {
            auto view = reg.view<Position, ObjectComponent, Plant, Terrain>();
            for (auto [entity, pos, obj, p, terrain] : view.each()) {
                // 年龄始终随生理时间增长，枯萎后也计入
                p.ageQuarters += t.quartersPerAp;

                // 致命地形：直接枯萎
                if (terrain == Terrain::Lethal) {
                    if (p.stage != PlantStage::Withered) {
                        p.stage = PlantStage::Withered;
                        p.elapsed = 0;
                    }
                    return;
                }
                // 枯萎后：只计时，不再恢复
                if (p.stage == PlantStage::Withered) {
                    ++p.elapsed;
                    return;
                }

                // 胁迫地形：停滞，累计胁迫时间，久则枯萎
                if (terrain == Terrain::Stressed) {
                    ++p.elapsed; // 用 elapsed 当作停滞计时
                    if (p.elapsed >= t.withered) {
                        p.stage = PlantStage::Withered;
                        p.elapsed = 0;
                    }
                    return;
                }

                // 适宜地形：正常推进
                ++p.elapsed;
                if (p.elapsed < p.duration(t)) return;
                p.elapsed = 0;
                switch (p.stage) {
                    case PlantStage::Seed:     p.stage = PlantStage::Sprout;   break;
                    case PlantStage::Sprout:   p.stage = PlantStage::Mature;   break;
                    case PlantStage::Mature:   p.stage = PlantStage::Fruiting; break;
                    case PlantStage::Fruiting: p.stage = PlantStage::Dropped;  break;
                    case PlantStage::Dropped:  p.stage = PlantStage::Seed;     break; // 回到种子
                    case PlantStage::Withered: break;
                }
                obj.curFrame = static_cast<uint8_t>(p.stage);
            }
        }
};
