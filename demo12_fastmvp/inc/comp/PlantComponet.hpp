#pragma once
#include <cstdint>

enum class PlantStage : uint8_t {
    Seed    = 0, // 休眠/种子
    Sprout  = 1, // 幼苗/营养生长
    Mature  = 2, // 成熟
    Fruiting= 3, // 挂果
    Dropped = 4, // 掉落
    Withered= 5  // 枯萎/死亡/薪柴
};

// 地形类别（由五因素组合后归并）
enum class Terrain {
    Suitable,  // 适宜：正常推进
    Stressed,  // 胁迫：停滞，久则枯萎
    Lethal     // 致命：直接枯萎
};

// 每个行动点 15 分钟，状态时长以行动点计
struct GrowthTiming {
    int seed     = 8;
    int sprout   = 24;
    int mature   = 48;
    int fruiting = 24;
    int dropped  = 4;
    int withered = 8;   // 枯萎后保留的行动点
    // 生理倍率：1 行动点 = 1 刻生理时间
    int quartersPerAp = 1;
};

struct Plant {
    PlantStage stage = PlantStage::Seed;
    int elapsed = 0;        // 当前状态已累计的行动点
    //int ageDays = 0;        // 生理年龄（天）
    int ageQuarters = 0;        // 生理年龄（一刻）
    // 状态时长查询
    int duration(const GrowthTiming& t) const {
        switch (stage) {
            case PlantStage::Seed:     return t.seed;
            case PlantStage::Sprout:   return t.sprout;
            case PlantStage::Mature:   return t.mature;
            case PlantStage::Fruiting: return t.fruiting;
            case PlantStage::Dropped:  return t.dropped;
            case PlantStage::Withered: return t.withered;
        }
        return 0;
    }
};

