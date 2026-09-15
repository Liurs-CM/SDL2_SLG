#pragma once
#include "core/Factories.hpp"
#include "sys/InventorySystem.hpp"

class SceneBootstrapSystem
{
    public:
        // 标记是否已完成初始化，防止 update 时重复执行
        bool initialized = false; 
        void init(entt::registry& registry) {
            if (initialized) return;
            // === 原始 makeObj 逻辑迁移至此 ===
            // 建议后续将坐标/ID提取为配置文件或数据表
            makeObj(registry, {13, 8}, "blocks_item", objID::Key, 2);
            makeObj(registry, {12, 8}, "blocks_item", objID::Coin, 2);
            makeObj(registry, {11, 8}, "blocks_item", objID::Coin, 2);
            makeObj(registry, {10, 8}, "blocks_item", objID::Coin, 2);
            makeObj(registry, {9, 8}, "blocks_item", objID::Cat, 2, 0, 1, 2);
            makeObj(registry, {8, 8}, "blocks_item", objID::Coin, 2);
            makeObj(registry, {7, 8}, "blocks_item", objID::ChestOpened, 2);
            auto boxEntity = makeObj(registry, {6, 8}, "blocks_item", objID::Chest, 2);
            makeObj(registry, boxEntity, "blocks_item", objID::Bag, 2);
            makeGoblin(registry, {8, 9}, "blocks_item", objID::Goblin, 2);
            makeObj(registry, {7, 9}, "blocks_item", objID::Cheese, 2);
            makeObj(registry, {6, 9}, "blocks_item", objID::DoorClosed, 2);
            makeObj(registry, {5, 9}, "blocks_item", objID::DoorOpend, 2);
            makeObj(registry, {8, 10}, "blocks_item", objID::Rabbit, 2);
            inventorySys().init(registry);
            makePlant(registry, {13, 9}, "blocks_item", objID::TreeApple, Plant{}, Terrain::Suitable, 6);
            initialized = true;
        }
};

