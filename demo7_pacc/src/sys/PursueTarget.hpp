#pragma once
#include "core/Maze.hpp"
#include <entt/entity/fwd.hpp>

// This system reads the target position from the Target component and pursues
// it by setting the DesiredDir component. Pacman doesn't use A*. Ghosts will
// turn in the direction that brings them closer to the player

void pursueTarget(entt::registry &, const MazeState &);
