#include "core/Factories.hpp"
#include "comp/CompGroup.hpp"

entt::entity makeObj(entt::registry &reg,
    vec Grid, std::string tilename, uint32_t firstTileID, 
    uint8_t numFrames, uint8_t curFrame, 
    uint8_t callbackID, uint8_t animSpeed)
{
    const auto entityObj = reg.create();
    reg.emplace<Position>(entityObj, Grid.x, Grid.y);
    reg.emplace<ObjectComponent>(entityObj, tilename, firstTileID, numFrames, curFrame, animSpeed, callbackID);
    std::cout << "make obj:" << tilename << " " << firstTileID << " " << Grid << "\n";
    return entityObj;
}

