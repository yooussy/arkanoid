#pragma once

#include <utility>
#include <vector>
#include "arkanoid.h"
#include "brick.h"

struct Carriage : Brick {
    float width = 0.0f;
    float height = 0.0f;
    ImColor color{255, 0, 0};
};
