#include "House.hpp"
#include "comp/House.hpp"
#include "comp/Target.hpp"
#include "comp/Position.hpp"
#include "core/Constants.hpp"
#include "comp/GhostMode.hpp"
#include "comp/HomePosition.hpp"
#include <entt/entity/registry.hpp>

void enterHouse(entt::registry &reg) {
    const auto view = reg.view<EnterHouse, Position, HomePosition>();
    for (const entt::entity e : view) {
        if (view.get<Position>(e).p == view.get<HomePosition>(e).home) {
            reg.remove<EnterHouse>(e);
            reg.emplace<LeaveHouse>(e);
            reg.remove<EatenMode>(e);
            reg.emplace<ChaseMode>(e);
        }
    }
}

void leaveHouse(entt::registry &reg) {
    auto view = reg.view<LeaveHouse, Position, Target>();
    for (const entt::entity e : view) {
        if (view.get<Position>(e).p == outsideHouse) {
            reg.remove<LeaveHouse>(e);
        } else {
            // leaveHouse is called after the set*Target systems so this will
            // overwrite the target position. It's a little bit hacky but it
            // works here.

            // We actually need to do this because ghosts can sometimes get stuck
            // in the house. If their target is towards the bottom of the maze,
            // they will move left and right without going up and out of the house
            view.get<Target>(e).p = outsideHouse;
        }
    }
}
