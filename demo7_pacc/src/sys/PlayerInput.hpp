#pragma once
#include "comp/Dir.hpp"
#include <SDL_scancode.h>
#include <entt/entity/fwd.hpp>

// Functions that read input should return whether they consumed the input.
// If an input function hasn't consumed an input, try the next one.
// Since there is only one input function in this game, we don't really
// need this but I thought I'd show it anyway.

// This takes the scancode of the key that was pressed and sets the DesiredDir
// of the player.

Dir readDir(const SDL_Scancode key);
bool playerInput(entt::registry &, Dir dir);
