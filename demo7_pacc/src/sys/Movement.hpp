#pragma once
#include "core/Maze.hpp"
#include <entt/entity/fwd.hpp>

// This moves the entity into the next tile
void movement(entt::registry &);

// This makes sure that the entity can move in the DesiredDir and sets
// the ActualDir to be the same
void wallCollide(entt::registry &, const MazeState &);
