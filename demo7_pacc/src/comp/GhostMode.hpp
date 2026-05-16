#pragma once
#include "core/Constants.hpp"

struct ChaseMode {};
struct ScatterMode {};
struct ScaredMode {
    int timer = ghostScaredTime;
};
struct EatenMode {};
