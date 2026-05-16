#pragma once
#include <entt/entity/fwd.hpp>

// These systems deal with ghosts entering and leaving the house
void enterHouse(entt::registry &);
void leaveHouse(entt::registry &);
