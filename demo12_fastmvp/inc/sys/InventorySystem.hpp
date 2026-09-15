#pragma once
#include "core/Factories.hpp"
#include "comp/CompGroup.hpp"
#include "entt/entt.hpp"
//#include <algorithm>
#include <iostream>

class InventorySystem {
    public:
        // ✅ 1. 获取单例实例（Meyers' Singleton，线程安全）
        static InventorySystem& instance() {
            static InventorySystem inst;
            return inst;
        }
        // ✅ 2. 禁止拷贝和移动，防止多实例
        InventorySystem(const InventorySystem&) = delete;
        InventorySystem& operator=(const InventorySystem&) = delete;
        InventorySystem(InventorySystem&&) = delete;
        InventorySystem& operator=(InventorySystem&&) = delete;

        bool addToInventory(entt::registry& registry, entt::entity player, entt::entity item)
        {
            auto& inventory = registry.get<Inventory>(player);
            uint8_t freeSlot = inventory.getFreeSlot();
            if (freeSlot == 255) { return false; } // 物品栏已满, 从世界中移除物品实体
            if (registry.valid(item)) {
                // 保存物品数据
                auto itemData = registry.get<ObjectComponent>(item);
                // 将物品实体引用存储到物品栏
                inventory.slots[freeSlot] = item;
                // 禁用物品的渲染和碰撞，但保留实体
                registry.remove<Position>(item);
                registry.remove<CollidableLayer>(item);
                registry.emplace<ContainedIn>(item, player, freeSlot);
                std::cout << "Picked up item id " << itemData.firstTileID << "\n";
                return true;
            }
            return false;
        }

        //void dropItem(entt::registry& registry, entt::entity player, int slot)
        void dropItem(entt::registry& registry, entt::entity player)
        {
            auto& inventory = registry.get<Inventory>(player);
            auto& playerPos = registry.get<Position>(player);
            const size_t slot = inventory.selectedSlot;
            if (slot < 0 || slot >= Inventory::MAX_SLOTS) return;
            entt::entity item = inventory.slots[slot];
            if (item == entt::null) return;
            // 检查该位置是否已被占用
            auto view = registry.view<Position>();
            bool positionOccupied = false;
            for (auto entity : view) {
                if (entity != player) {
                    auto& pos = view.get<Position>(entity);
                    if (pos == playerPos) {
                        positionOccupied = true;
                        break;
                    }
                }
            }
            if (!positionOccupied) {
                // 恢复物品的位置和碰撞
                registry.emplace_or_replace<Position>(item, playerPos);
                registry.emplace_or_replace<CollidableLayer>(item);
                registry.remove<ContainedIn>(item);
                // 清空物品栏槽位
                inventory.slots[slot] = entt::null;
                std::cout << "Dropped item at " << playerPos << "\n";
            } else {
                std::cout << "Cannot drop item here - position occupied" << "\n";
            }
        }

        void cycleSlot(entt::registry& registry, entt::entity player, int direction) noexcept
        {
            auto* inv = registry.try_get<Inventory>(player);
            if (!inv) return; // 无背包则静默退出，符合 ECS "缺失即忽略" 原则
            // 1. 安全清除旧 slot 高亮
            if (auto* oldObj = registry.try_get<ObjectComponent>(inv->getCurrentSlotBox()); oldObj) {
                oldObj->firstTileID = uiID::Box;
            }
            // 2. 切换 slot
            inv->cycleSlot(direction);
            // 3. 安全设置新 slot 高亮
            if (auto* newObj = registry.try_get<ObjectComponent>(inv->getCurrentSlotBox()); newObj) {
                newObj->firstTileID = uiID::BoxCur;
            }
        }

        bool removeFromInventory(entt::registry& registry, entt::entity player, uint8_t slot)
        {
            // 1. 安全获取背包组件
            auto* inventory = registry.try_get<Inventory>(player);
            if (!inventory || slot >= Inventory::MAX_SLOTS) return false;
            // 2. 验证实体有效性（防止已销毁的脏引用）
            entt::entity item = inventory->slots[slot];
            if (item == entt::null || !registry.valid(item)) {
                inventory->slots[slot] = entt::null; // 顺手清理脏数据
                return false;
            }
            // 3. 清除 UI 高亮状态（如果当前选中的是被删除的槽位）
            if (slot == inventory->selectedSlot) {
                if (auto* obj = registry.try_get<ObjectComponent>(item)) {
                    obj->firstTileID = uiID::Box; // 恢复为普通框
                }
            }
            // 4. 移除 ContainedIn 标记（保持 ECS 关系完整性）
            registry.remove<ContainedIn>(item);
            // 5. 彻底销毁物品实体（释放所有组件与内存）
            registry.emplace<PendingDestroy>(item);
            // 6. 清空槽位引用
            inventory->slots[slot] = entt::null;
            std::cout << "Removed consumable from slot " << static_cast<int>(slot) << "\n";
            return true;
        }

        void init(entt::registry& registry) const {
            for (int i = 0; i < Inventory::MAX_SLOTS; i++) {
                auto playerEntity  = registry.ctx().get<PlayerRef>().id;
                auto& inventory = registry.get<Inventory>(playerEntity);
                inventory.slotBoxs[i] = makeUI(registry, {8 + i, -1}, "blocks_ui", uiID::Box);
                //makeUI(registry, {8 + i, -1}, "blocks_ui", uiID::Box);
            }
            //makeUI(registry, {12, -1}, "blocks_ui", uiID::Box);
            makeUI(registry, {0, 0}, "blocks_ui", uiID::CornerLU);
            makeUI(registry, {1, 0}, "blocks_ui", uiID::CornerU);
            makeUI(registry, {2, 0}, "blocks_ui", uiID::CornerU);
            makeUI(registry, {3, 0}, "blocks_ui", uiID::CornerU);
            makeUI(registry, {0, 1}, "blocks_ui", uiID::CornerL);
        }

    private:
        // ✅ 3. 私有构造函数，阻止外部 new
        InventorySystem() = default;
};

inline InventorySystem& inventorySys() { return InventorySystem::instance(); }
