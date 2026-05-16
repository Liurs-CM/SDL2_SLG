#pragma once
#include "Dir.hpp"
#include "Pos.hpp"

constexpr Pos toPos(const Dir dir, const int length = 1) {
    switch (dir) {
        case Dir::up:    return {0, -1 * length};
        case Dir::right: return {1 * length, 0};
        case Dir::down:  return {0, 1 * length};
        case Dir::left:  return {-1 * length, 0};
        default:         return {0, 0};
    }
}
