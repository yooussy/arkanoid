#include "arkanoid_impl.h"
#include <GLFW/glfw3.h>

#ifdef USE_ARKANOID_IMPL
Arkanoid* create_arkanoid() {
    return new ArkanoidImpl();
}
#endif

void ArkanoidImpl::reset(const ArkanoidSettings& settings) {
    world_size.x = settings.world_size[0];
    world_size.y = settings.world_size[1];

    reset_carriage(settings);
    reset_balls(settings);
    reset_bricks(settings);
}

void ArkanoidImpl::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) {
    world_to_screen = Vect(io.DisplaySize.x / world_size.x * 3.0f / 4.0f, io.DisplaySize.y / world_size.y);

    update_ball(io, debug_data, elapsed);
    update_bricks(io, debug_data, elapsed);
    update_carriage(io, debug_data, elapsed);
}

void ArkanoidImpl::draw(ImGuiIO& io, ImDrawList& draw_list) {
    draw_ball(io, draw_list);
    draw_bricks(io, draw_list);
    draw_carriage(io, draw_list);
}

void ArkanoidImpl::add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos,
                                      const Vect& normal) {
    ArkanoidDebugData::Hit hit;
    hit.screen_pos = world_pos * world_to_screen;
    hit.normal = normal;
    debug_data.hits.push_back(std::move(hit));
}
