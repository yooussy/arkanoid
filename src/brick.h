#pragma once

#include "arkanoid.h"

struct Brick {
    ImColor color = ImColor(0, 128, 0);
    BrickType type;
    int hit_count = 0;
    Vect p_min{0, 0};
    Vect p_max{0, 0};

    Brick() = default;

    Brick(Vect p_min, Vect p_max, BrickType type): p_min(p_min), p_max(p_max), type(type) {
        switch (type) {
            case BrickType::regular : color = ImColor(51, 102, 0); break;
            case BrickType::strong : color = ImColor(128, 12, 255); break;
            case BrickType::double_ball : color = ImColor(255, 165, 0); break;
            case BrickType::move_down : color = ImColor(0, 0, 255); break;
        }
    }
};
