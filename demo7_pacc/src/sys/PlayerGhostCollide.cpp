#include "PlayerGhostCollide.hpp"
#include "comp/Dir.hpp"
#include "comp/Ghost.hpp"
#include "comp/Player.hpp"
#include "comp/Position.hpp"
#include "util/Dir2Pos.hpp"
#include "comp/GhostMode.hpp"
#include <entt/entity/registry.hpp>

namespace {

    bool collide(
            const Pos pPos,
            const Pos gPos,
            const Dir pDir,
            const Dir gDir
            ) {
        if (pPos == gPos)               return true;
        if (pPos + toPos(pDir) != gPos) return false;
        if (pDir != opposite(gDir))     return false;
        return true;
    }

}

GhostCollision playerGhostCollide(entt::registry &reg) {
    const auto players = reg.view<Player, Position, ActualDir>();
    const auto ghosts = reg.view<Ghost, Position, ActualDir>();
    for (const entt::entity p : players) {
        const Pos playerPos = players.get<Position>(p).p;
        const Dir playerDir = players.get<ActualDir>(p).d;
        for (const entt::entity g : ghosts) {
            const Pos ghostPos = ghosts.get<Position>(g).p;
            const Dir ghostDir = ghosts.get<ActualDir>(g).d;
            if (collide(playerPos, ghostPos, playerDir, ghostDir)) {
                if (reg.has<ScaredMode>(g)) {
                    return {g, GhostCollision::Type::eat};
                } else if (reg.has<EatenMode>(g)) {
                    continue;
                }
                return {g, GhostCollision::Type::lose};
            }
        }
    }
    return {entt::null, GhostCollision::Type::none};
}
