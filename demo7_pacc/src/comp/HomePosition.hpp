#pragma once
#include "util/Pos.hpp"

struct HomePosition {
  Pos home;    // position in the ghost house
  Pos scatter; // unreachable point in a corner
};
