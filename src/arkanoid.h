#pragma once

#include <vector>
#include "base.h"
#include "gamestate.h"

enum class BrickType {regular, strong, double_ball, move_down};

struct ArkanoidSettings {
    static constexpr int bricks_rows_min = 3;
    static constexpr int bricks_rows_max = 10;
    static constexpr int bricks_columns_min = 10;
    static constexpr int bricks_columns_max = 30;

    static constexpr float bricks_rows_padding_min = 5.0f;
    static constexpr float bricks_rows_padding_max = 20.0f;
    static constexpr float bricks_columns_padding_min = 5.0f;
    static constexpr float bricks_columns_padding_max = 20.0f;

    static constexpr float bricks_width_min = 1.0f;
    static constexpr float bricks_width_max = 50.0f;
    static constexpr float bricks_height_min = 1.0f;
    static constexpr float bricks_height_max = 50.0f;

    static constexpr float ball_radius_min = 5.0f;
    static constexpr float ball_radius_max = 50.0f;
    static constexpr float ball_speed_min = 1.0f;
    static constexpr float ball_speed_max = 5000.0f;

    static constexpr float carriage_width_min = 5.0f;
    static constexpr float carriage_width_max = 100.0f;

    Vect world_size = Vect(1280.0f, 720.0f);

    int bricks_rows_count = 10;
    int bricks_columns_count = 21;

    float bricks_columns_padding = 5.0f;
    float bricks_rows_padding = 5.0f;

    float bricks_width = 50.0f;
    float bricks_height = 10.0f;

    float ball_radius = 10.0f;
    float ball_speed = 300.0f;

    float carriage_width = 100.0f;
};

struct ArkanoidDebugData {
    struct Hit {
        Vect screen_pos;    // Hit position, in screen space
        Vect normal;        // Hit normal
        float time = 0.0f;  // leave it default
    };

    std::vector<Hit> hits;
};

class Arkanoid {
public:
    virtual ~Arkanoid() = default;
    virtual void reset(const ArkanoidSettings& settings) = 0;
    virtual void draw(ImGuiIO& io, ImDrawList& draw_list) = 0;
    virtual void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) = 0;
    GameState game_state;
};

extern Arkanoid* create_arkanoid();
