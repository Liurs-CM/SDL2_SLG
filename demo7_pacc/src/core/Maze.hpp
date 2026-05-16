#pragma once
#include <cstdint>
#include "util/Grid.hpp"

enum class Tile : std::uint8_t {
    empty,
    dot,
    energizer,
    wall,
    door
};

using MazeState = Grid<Tile>;

MazeState makeMazeState();
