#include "arkanoid.h"
#include "arkanoid_impl.h"

void ArkanoidImpl::reset_world(const ArkanoidSettings& settings) {
    world_size.x = settings.world_size[0];
    world_size.y = settings.world_size[1];
    game_state.current_score = 0;

}

void ArkanoidImpl::reset_balls(const ArkanoidSettings& settings) {
    balls_list.clear();
    balls_list.push_back(Ball{carriage.p_min, Vect(settings.ball_speed, settings.ball_speed * -1.0f), 
                                                   settings.ball_radius, settings.ball_speed});
}

void ArkanoidImpl::reset_bricks(const ArkanoidSettings& settings) {
    is_moving_down = false;

    bricks_rows_count = settings.bricks_rows_count;
    bricks_columns_count = settings.bricks_columns_count;

    bricks_width = settings.bricks_width;
    bricks_height = settings.bricks_height;

    bricks_rows_padding = settings.bricks_rows_padding;
    bricks_columns_padding = settings.bricks_columns_padding;

    while (bricks_columns_count * bricks_width + (bricks_columns_count - 1) * bricks_columns_padding > 
                                                        world_size.x) {
        bricks_width -= 5.0f;
    }

    // delete old layout
    bricks_list.clear();
    is_moving_down = false;

    // make new layout
    Vect first_brick_position = {(world_size.x - bricks_width * bricks_columns_count -
                                  bricks_columns_padding * (bricks_columns_count - 1)) / 2, 100};

    Vect p_min = first_brick_position;
    Vect p_max = first_brick_position + Vect(bricks_width, bricks_height);

    for (int row = 0; row < bricks_rows_count; ++row) {
        for (int column = 0; column < bricks_columns_count; ++column) {
            if (row == 0 || row == 2) {
                bricks_list.push_back(Brick{p_min, p_max, BrickType::strong});
            } else if (row == 1 && (column % 2 == 1)) {
                bricks_list.push_back(Brick{p_min, p_max, BrickType::double_ball});
            } else if (row == bricks_rows_count / 2 && column == bricks_columns_count / 2) {
                bricks_list.push_back(Brick{p_min, p_max, BrickType::move_down});
            } else {
                bricks_list.push_back(Brick{p_min, p_max, BrickType::regular});
            }
            p_min.x += bricks_width + bricks_columns_padding;
            p_max.x += bricks_width + bricks_columns_padding;
        }
        p_min.x = first_brick_position.x;
        p_min.y += bricks_height + bricks_rows_padding;
        p_max.x = first_brick_position.x + bricks_width;
        p_max.y += bricks_height + bricks_rows_padding;
    }
}

void ArkanoidImpl::reset_carriage(const ArkanoidSettings& settings) {
    //calculate starting position of carriage
    carriage.width = settings.carriage_width;

    carriage.p_min = Vect(world_size.x / 2 - carriage.width / 2, world_size.y * 7.0f / 8.0f);
    carriage.p_max = carriage.p_min + Vect(carriage.width, 10.0f);
}
