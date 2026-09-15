#pragma once
#include "util/Vector2D.h"
#include "entt/entt.hpp"
#include "comp/PlantComponet.hpp"

//entt::entity makePlayer(entt::registry &);

//entt::entity makeObj(entt::registry &reg);
entt::entity makeObj(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames = 1, uint8_t curFrame = 0, 
    uint8_t callbackID = 0, uint8_t animSpeed = 2);

entt::entity makeObj(entt::registry &reg,
    entt::entity box, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames = 1, uint8_t curFrame = 0, 
    uint8_t callbackID = 0, uint8_t animSpeed = 2);

entt::entity makeGoblin(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames = 1, uint8_t curFrame = 0, 
    uint8_t callbackID = 0, uint8_t animSpeed = 2);

entt::entity makeUI(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames = 1, uint8_t curFrame = 0, 
    uint8_t callbackID = 0, uint8_t animSpeed = 2);

entt::entity makePlant(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    Plant plant, Terrain terrain,
    uint8_t numFrames = 1, uint8_t curFrame = 0, 
    uint8_t callbackID = 0, uint8_t animSpeed = 2);
