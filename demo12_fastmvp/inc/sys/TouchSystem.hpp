#pragma once
#include "comp/CompGroup.hpp"
#include "sys/UIMsgSystem.hpp"
#include "util/Vector2D.h"
#include <entt/entt.hpp>

class TouchSystem 
{
    public:
        static void touch(entt::registry& registry, entt::entity playerEntity, vec InteractPos, UIMsgSystem& UIMsgSys) 
        {
            //auto playerView = registry.view<SpriteComponent, PlayerTag>();
            auto objView = registry.view<ObjectComponent, Position>();
            for (auto [ObjEntity, obj, pos] : objView.each()) {
                //std::cout << "chk " << obj.firstTileID << "...at " << pos << " vs. " << InteractPos << "\n";
                if (pos == InteractPos) {
                    switch(obj.firstTileID) {
                        case objID::Chest: 
                            //std::cout << "Interact Chest" << "\n";
                            UIMsgSys.setInfoMsg("It's locked and I don't have a key.");
                            break;
                        case objID::Coin: 
                            //std::cout << "Interact Coin" << "\n";
                            registry.get<Sources>(playerEntity).gold += 5;
                            registry.emplace<PendingDestroy>(ObjEntity);
                            UIMsgSys.setInfoMsg("Picked up a gold coin.");
                            UIMsgSys.setInfoMsg("Found 5 gold coins!");
                            break;
                        default:
                            //std::cout << "not Found obj InteractPos" << "\n";
                            UIMsgSys.setInfoMsg("id[%d] not yet interactable!", obj.firstTileID);
                            break;
                    }
                }
            }
        }
};
