#pragma once 
#include "core/Maze.hpp"
#include <entt/entity/fwd.hpp>

// Returns the number of dots that the player ate (usually 0 or 1)
int eatDots(entt::registry &, MazeState &);

// Returns whether the player collided with an energizer
bool eatEnergizer(entt::registry &, MazeState &);
