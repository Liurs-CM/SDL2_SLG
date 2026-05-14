#pragma once
#include "core/maze.hpp"
#include "util/sprites.hpp"
#include <entt/entity/fwd.hpp>
#include "util/sdl_quad_writer.hpp"

void playerRender(entt::registry &, SDL::QuadWriter &, int);
void ghostRender(entt::registry &, SDL::QuadWriter &, int);
void dotRender(SDL::QuadWriter &, const MazeState &);
void fullRender(SDL::QuadWriter &, animera::SpriteID);
void bulletRender(entt::registry &, SDL::QuadWriter &, int);
