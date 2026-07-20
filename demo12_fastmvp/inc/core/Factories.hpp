#pragma once
#include "util/Vector2D.h"
#include "entt/entt.hpp"

//entt::entity makePlayer(entt::registry &);

//entt::entity makeObj(entt::registry &reg);
entt::entity makeObj(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames, uint8_t curFrame, 
    uint8_t callbackID, uint8_t animSpeed);
