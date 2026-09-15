#include "core/Factories.hpp"
#include "comp/CompGroup.hpp"

entt::entity makeObj(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames, uint8_t curFrame, 
    uint8_t callbackID, uint8_t animSpeed)
{
    const auto entityObj = reg.create();
    reg.emplace<Position>(entityObj, Grid);
    reg.emplace<ObjectComponent>(entityObj, tilename, firstTileID, numFrames, curFrame, animSpeed, callbackID);
    reg.emplace<CollidableLayer>(entityObj);
    std::cout << "make obj:" << tilename << " " << firstTileID << " " << Grid << "\n";
    return entityObj;
}

entt::entity makeObj(entt::registry &reg,
    entt::entity box, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames, uint8_t curFrame, 
    uint8_t callbackID, uint8_t animSpeed)
{
    const auto entityObj = reg.create();
    reg.emplace<ContainedRef>(entityObj, box);
    reg.emplace<ObjectComponent>(entityObj, tilename, firstTileID, numFrames, curFrame, animSpeed, callbackID);
    std::cout << "make contained obj:" << tilename << " " << firstTileID << "\n";
    return entityObj;
}

entt::entity makeGoblin(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames, uint8_t curFrame, 
    uint8_t callbackID, uint8_t animSpeed)
{
    const auto entityObj = reg.create();
    reg.emplace<Position>(entityObj, Grid);
    reg.emplace<ObjectComponent>(entityObj, tilename, firstTileID, numFrames, curFrame, animSpeed, callbackID);
    reg.emplace<CollidableLayer>(entityObj);
    reg.emplace<GoblinAI>(entityObj);
    std::cout << "make obj:" << tilename << " " << firstTileID << " " << Grid << "\n";
    return entityObj;
}

entt::entity makeUI(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames, uint8_t curFrame, 
    uint8_t callbackID, uint8_t animSpeed)
{
    const auto entityObj = reg.create();
    reg.emplace<uiPosition>(entityObj, Grid);
    reg.emplace<ObjectComponent>(entityObj, tilename, firstTileID, numFrames, curFrame, animSpeed, callbackID);
    std::cout << "make ui:" << tilename << " " << firstTileID << " " << Grid << "\n";
    return entityObj;
}

entt::entity makePlant(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    Plant plant, Terrain terrain,
    uint8_t numFrames, uint8_t curFrame, 
    uint8_t callbackID, uint8_t animSpeed)
{
    const auto entityObj = reg.create();
    reg.emplace<Position>(entityObj, Grid);
    reg.emplace<ObjectComponent>(entityObj, tilename, firstTileID, numFrames, curFrame, animSpeed, callbackID);
    reg.emplace<CollidableLayer>(entityObj);
    reg.emplace<Plant>(entityObj, plant);
    reg.emplace<Terrain>(entityObj, terrain);
    std::cout << "make plant:" << tilename << " " << firstTileID << " " << Grid << "\n";
    return entityObj;
}

