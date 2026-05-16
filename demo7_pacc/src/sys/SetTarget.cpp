#include "SetTarget.hpp"
#include "CanMove.hpp"
#include "comp/Dir.hpp"
#include "comp/Target.hpp"
#include "comp/Position.hpp"
#include "util/Dir2Pos.hpp"
#include "comp/GhostMode.hpp"
#include "comp/ChaseTarget.hpp"
#include "comp/HomePosition.hpp"
#include <entt/entity/registry.hpp>

void setClydeChaseTarget(entt::registry &reg) {
    auto view = reg.view<
        Target, Position, ChaseMode, ClydeChaseTarget, HomePosition
        >();
    for (const entt::entity e : view) {
        const entt::entity player = view.get<ClydeChaseTarget>(e).player;
        const Pos playerPos = reg.get<Position>(player).p;
        const float dist = distance(playerPos, view.get<Position>(e).p);
        if (dist >= 8.0f) {
            view.get<Target>(e).p = playerPos;
        } else {
            view.get<Target>(e).p = view.get<HomePosition>(e).scatter;
        }
    }
}

void setScatterTarget(entt::registry &reg) {
    auto view = reg.view<Target, ScatterMode, HomePosition>();
    for (const entt::entity e : view) {
        view.get<Target>(e).p = view.get<HomePosition>(e).scatter;
    }
}

void setScaredTarget(entt::registry &reg, const MazeState &maze, std::mt19937 &rand) {
    auto view = reg.view<Target, Position, ScaredMode, ActualDir>();
    for (const entt::entity e : view) {
        const Pos pos = view.get<Position>(e).p;
        const Dir dir = view.get<ActualDir>(e).d;
        const Pos nextPos = pos + toPos(dir);
        std::uniform_int_distribution<int> dist(0, 3);
        Dir candDir = static_cast<Dir>(dist(rand));
        Pos candPos = nextPos;

        for (int i = 0; i != 4; ++i) {
            if (candDir == opposite(dir)) {
                candDir = rotateCW(candDir);
                continue;
            }

            if (!canMove(reg, maze, e, nextPos, candDir)) {
                candDir = rotateCW(candDir);
                continue;
            }

            candPos = nextPos + toPos(candDir);
            break;
        }

        view.get<Target>(e).p = candPos;
    }
}

void setEatenTarget(entt::registry &reg) {
    auto view = reg.view<Target, EatenMode, HomePosition>();
    for (const entt::entity e : view) {
        view.get<Target>(e).p = view.get<HomePosition>(e).home;
    }
}
