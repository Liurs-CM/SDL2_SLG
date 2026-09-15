#pragma once
#include "comp/CompGroup.hpp"
#include "sys/UIMsgSystem.hpp"
#include "sys/InventorySystem.hpp"
#include "util/Vector2D.h"
#include <entt/entt.hpp>

// 将交互逻辑抽离为独立策略或函数对象，避免 touch 函数膨胀
namespace InteractionHandlers 
{
    static void handleChest(entt::registry& registry, entt::entity playerEntity,
            UIMsgSystem& uiMsg, entt::entity ObjEntity)
    {
        // 安全获取：避免循环内重复查找，且防止组件缺失崩溃
        if (const auto* inv = registry.try_get<Inventory>(playerEntity); inv) {
            auto slotIndex = inv->findSlotIndex(registry, objID::Key);
            if (slotIndex != static_cast<size_t>(-1)) {
                inventorySys().removeFromInventory(registry, playerEntity, slotIndex);
                auto& obj = registry.get<ObjectComponent>(ObjEntity);
                auto pos = registry.get<Position>(ObjEntity);
                obj.firstTileID = objID::ChestOpened;
                uiMsg.setInfoMsg("Open chest, key used.");
                // 显示宝箱物品
                auto view = registry.view<const ContainedRef>();
                for (auto [itemEntity, box] : view.each()) {
                    if(box == ObjEntity) {
                        auto itemobj = registry.get<ObjectComponent>(itemEntity);
                        uiMsg.setInfoMsg("Show item in box. %d", itemobj.firstTileID);
                        if(inventorySys().addToInventory(registry, playerEntity, itemEntity)){
                            registry.remove<ContainedRef>(itemEntity);
                        } else {
                            registry.emplace<Position>(itemEntity, pos - vec::unit_x);
                            registry.emplace<CollidableLayer>(itemEntity);
                        }
                    }
                }
            } else {
                uiMsg.setInfoMsg("It's locked and I don't have a key.");
            }
        } else {
            uiMsg.setInfoMsg("Error: Player has no inventory.");
        }
    }

    static void handleCoin(entt::registry& registry, entt::entity playerEntity,
            UIMsgSystem& uiMsg, entt::entity ObjEntity)
    {
        //std::cout << "Interact Coin" << "\n";
        if(inventorySys().addToInventory(registry, playerEntity, ObjEntity)){
            //registry.get<Sources>(playerEntity).gold += 5;
            uiMsg.setInfoMsg("Picked up a coin.");
            // TODO: 销毁 Coin 实体 registry.destroy(objEntity);
        } else {
            uiMsg.setInfoMsg("Inventory bag full (*_*)!");
        }
    }

    static void handleKey(entt::registry& registry, entt::entity playerEntity,
            UIMsgSystem& uiMsg, entt::entity ObjEntity)
    {
        if(inventorySys().addToInventory(registry, playerEntity, ObjEntity)){
            uiMsg.setInfoMsg("Picked up a key.");
        } else {
            uiMsg.setInfoMsg("Inventory bag full (*_*)!");
        }
    }

    static void handleGoblin(entt::registry& registry, entt::entity playerEntity,
            UIMsgSystem& uiMsg, entt::entity ObjEntity)
    {
        auto* ai = registry.try_get<GoblinAI>(ObjEntity);
        if (!ai) return;
        switch (ai->state) {
            case GoblinState::Init: 
                uiMsg.setInfoMsg("Prisoner: Excuse me, do you mind if I just squeeze past?");
                uiMsg.setInfoMsg("Goblin: Go away! I'm meant to be guarding this magical 'House' icon from the contestant. But I'm in a bad mood because I left my lunch in the fridge today, and I'm really hungry.");
                uiMsg.setInfoMsg("Goblin: Now I'm going to have to wait until I get home, but I'm on an extended shift here so that's hours away. Stupid job, stupid contract.");
                uiMsg.setInfoMsg("Goblin: So, unless you've got something to eat, you can just push off.");
                ai->state = GoblinState::WantFood;
                break;
            case GoblinState::WantFood:
                if (const auto* inv = registry.try_get<Inventory>(playerEntity); inv) {
                    auto slotIndex = inv->findSlotIndex(registry, objID::Cheese);
                    if (slotIndex != static_cast<size_t>(-1)) {
                        inventorySys().removeFromInventory(registry, playerEntity, slotIndex);
                        uiMsg.setInfoMsg("Prisoner: Will this do?");
                        uiMsg.setInfoMsg("Goblin: Cheese! Wow, and it's a big lump, too!");
                        uiMsg.setInfoMsg("Goblin: Thanks, stranger, I'm going to go and enjoy this. Back soon.");
                        registry.emplace<PendingDestroy>(ObjEntity);
                    } else {
                        uiMsg.setInfoMsg("Goblin: Unless you've got some food, I don't want to talk to you.");
                    }
                }
                break;
            default:
                break;
        }
    }

    static void handleCheese(entt::registry& registry, entt::entity playerEntity,
            UIMsgSystem& uiMsg, entt::entity ObjEntity)
    {
        if(inventorySys().addToInventory(registry, playerEntity, ObjEntity)){
            uiMsg.setInfoMsg("Picked up a cheese.");
        } else {
            uiMsg.setInfoMsg("Inventory bag full (*_*)!");
        }
    }

    static void handleRabbit(entt::registry& registry, entt::entity playerEntity,
            UIMsgSystem& uiMsg/*, entt::entity ObjEntity*/)
    {
        auto& inventory = registry.get<Inventory>(playerEntity);
        if(inventory.hasItem(registry, objID::Sword)){
            uiMsg.setInfoMsg("Attack rabbit.");
        } else {
            uiMsg.setInfoMsg("No weapon");
        }
    }

}

class TouchSystem 
{
    public:
        static void touch(entt::registry& registry, entt::entity playerEntity, vec InteractPos, UIMsgSystem& UIMsgSys) 
        {
            // 【关键优化】循环外缓存玩家组件指针，避免 N 次重复查找
            const auto* playerInv = registry.try_get<Inventory>(playerEntity);
            if (!playerInv) { UIMsgSys.setInfoMsg("System Error: Invalid player entity."); return; }
            auto objView = registry.view<ObjectComponent, Position>();
            for (auto [ObjEntity, obj, pos] : objView.each()) {
                //std::cout << "chk " << obj.firstTileID << "...at " << pos << " vs. " << InteractPos << "\n";
                if (pos != InteractPos) continue;
                switch(obj.firstTileID) {
                    case objID::Chest: 
                        InteractionHandlers::handleChest(registry, playerEntity, UIMsgSys, ObjEntity);
                        break;
                    case objID::Coin: 
                        InteractionHandlers::handleCoin(registry, playerEntity, UIMsgSys, ObjEntity);
                        break;
                    case objID::Key: 
                        InteractionHandlers::handleKey(registry, playerEntity, UIMsgSys, ObjEntity);
                        break;
                    case objID::Goblin: 
                        InteractionHandlers::handleGoblin(registry, playerEntity, UIMsgSys, ObjEntity);
                        break;
                    case objID::Cheese: 
                        InteractionHandlers::handleCheese(registry, playerEntity, UIMsgSys, ObjEntity);
                        break;
                    case objID::Rabbit: 
                        InteractionHandlers::handleRabbit(registry, playerEntity, UIMsgSys/*, ObjEntity*/);
                        break;
                    default:
                        //std::cout << "not Found obj InteractPos" << "\n";
                        UIMsgSys.setInfoMsg("id[%d] not yet interactable!", obj.firstTileID);
                        break;
                }
            }
        }
};
