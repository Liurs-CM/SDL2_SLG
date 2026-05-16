#pragma once
#include "core/Maze.hpp"
#include "util/Sprites.hpp"
#include <entt/entity/fwd.hpp>
#include "util/SDL_QuadWriter.hpp"

// Render the player (the yellow guy)
void playerRender(entt::registry &, SDL::QuadWriter &, int);

// Render the ghosts
void ghostRender(entt::registry &, SDL::QuadWriter &, int);

// Render the dots and energizers
void dotRender(SDL::QuadWriter &, const MazeState &);

// Render a sprite that covers the whole screen (maze, win, lose)
void fullRender(SDL::QuadWriter &, animera::SpriteID);
