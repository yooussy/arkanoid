#pragma once

#include <list>
#include "arkanoid.h"
#include "ball.h"
#include "brick.h"
#include "carriage.h"

#define USE_ARKANOID_IMPL

class ArkanoidImpl : public Arkanoid {
public:
    void reset(const ArkanoidSettings& settings) override;
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;
    void draw(ImGuiIO& io, ImDrawList& draw_list) override;

private:
    void reset_balls(const ArkanoidSettings& settings);
    void reset_bricks(const ArkanoidSettings& settings);
    void reset_carriage(const ArkanoidSettings& settings);
    void reset_world(const ArkanoidSettings& setting);

    void update_ball(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void update_bricks(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void update_carriage(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);

    void draw_ball(ImGuiIO& io, ImDrawList& draw_list);
    void draw_bricks(ImGuiIO& io, ImDrawList& draw_list);
    void draw_carriage(ImGuiIO& io, ImDrawList& draw_list);

    void add_debug_hit(ArkanoidDebugData& debug_data, const Vect& pos, const Vect& normal);

    Vect world_size;
    Vect world_to_screen;

    int bricks_rows_count;
    int bricks_columns_count;

    float bricks_columns_padding;
    float bricks_rows_padding;

    float bricks_width;
    float bricks_height;
    bool is_moving_down;
    
    std::list<Brick> bricks_list;
    std::list<Ball> balls_list;
    Carriage carriage;
};
