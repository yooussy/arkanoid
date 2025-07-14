#include "arkanoid_impl.h"

void ArkanoidImpl::draw_ball(ImGuiIO& io, ImDrawList& draw_list) {
    for (const auto& ball : balls_list) {
        Vect screen_pos = ball.position * world_to_screen;
        float screen_radius = ball.radius * world_to_screen.x;
        draw_list.AddCircleFilled(screen_pos, screen_radius, ImColor(100, 255, 100));
    }
}

void ArkanoidImpl::draw_bricks(ImGuiIO& io, ImDrawList& draw_list) {
    for (const auto& brick : bricks_list) {
        Vect screen_pos = brick.p_min * world_to_screen;
        Vect screen_p_min = brick.p_min * world_to_screen;
        Vect screen_p_max = brick.p_max * world_to_screen;
        draw_list.AddRectFilled(screen_p_min, screen_p_max, brick.color);
    }
}

void ArkanoidImpl::draw_carriage(ImGuiIO& io, ImDrawList& draw_list) {
    Vect screen_pos = carriage.p_min * world_to_screen;
    Vect screen_p_min = carriage.p_min * world_to_screen;
    Vect screen_p_max = carriage.p_max * world_to_screen;
    draw_list.AddRectFilled(screen_p_min, screen_p_max, carriage.color);
}
