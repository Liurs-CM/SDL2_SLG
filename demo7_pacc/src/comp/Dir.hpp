#pragma once
#include "util/Dir.hpp"

// The direction that the entity wants to go
struct DesiredDir {
    Dir d = Dir::none;
};

// The direction that the entity is actually going in
struct ActualDir {
    Dir d = Dir::none;
};
