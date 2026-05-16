#pragma once
#include "util/Dir.hpp"
#include "core/Maze.hpp"
#include <entt/entity/fwd.hpp>

// Checks whether an entity can move in some direction.
// Deals with ghosts entering and leaving the house
bool canMove(entt::registry &, const MazeState &, entt::entity, Pos, Dir);
