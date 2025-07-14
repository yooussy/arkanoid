#include "arkanoid.h"
#include "arkanoid_impl.h"
#include "brick.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

void ArkanoidImpl::update_ball(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) {
    std::list<decltype(balls_list)::iterator> balls_to_delete;

    for (auto ball = balls_list.begin(); ball != balls_list.end(); ++ball) {

        // update ball position according
        // its velocity and elapsed time
        ball->position += ball->velocity * elapsed;

        if (ball->position.x < ball->radius) {
            ball->position.x += (ball->radius - ball->position.x) * 2.0f;
            ball->velocity.x *= -1.0f;

            add_debug_hit(debug_data, Vect(0, ball->position.y), Vect(1, 0));
        } else if (ball->position.x > (world_size.x - ball->radius)) {
            ball->position.x -= (ball->position.x - (world_size.x - ball->radius)) * 2.0f;
            ball->velocity.x *= -1.0f;

            add_debug_hit(debug_data, Vect(world_size.x, ball->position.y), Vect(-1, 0));
        }

        if (ball->position.y < ball->radius) {
            ball->position.y += (ball->radius - ball->position.y) * 2.0f;
            ball->velocity.y *= -1.0f;

            add_debug_hit(debug_data, Vect(ball->position.x, 0), Vect(0, 1));
        } else if (ball->position.y > (world_size.y - ball->radius)) {
            ball->position.y -= (ball->position.y - (world_size.y - ball->radius)) * 2.0f;
            ball->velocity.y *= -1.0f;

            add_debug_hit(debug_data, Vect(ball->position.x, world_size.y), Vect(0, -1));
        }

        if (ball->position.y > carriage.p_max.y) {
            balls_to_delete.push_back(ball);
        }
    }
    // delete balls, which appeared under the carriage
    if (!game_state.is_debug_mode) {
        for (auto& ball : balls_to_delete) {
            balls_list.erase(ball);
        }
    }

    if (balls_list.size() == 0) {
        game_state.is_over = true;
    }
}

void ArkanoidImpl::update_bricks(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) {
    if (is_moving_down == true) {
        for (auto& brick : bricks_list) {
            brick.p_min.y += 0.1f;
            brick.p_max.y += 0.1f;
        }
    }

    for (auto& ball : balls_list) {
        // using continious collision detection to avoid tunneling
        ball.next_position = ball.position + ball.velocity * elapsed;

        decltype(bricks_list)::iterator brick_to_delete;
        bool delete_brick = false;
        for (auto brick = bricks_list.begin(); brick != bricks_list.end(); ++brick) {
            // making line between current position and next position of ball
            // line: (1 - t) * curr_pos + t * next_pos
            float t = 0.0f;
            Vect point{0.0f};
            Vect normal{0.0f};
            while (t < 1.0f) {
                point = (1.0f - t) * ball.position + t * ball.next_position;
                // check if ball touched brick
                // check with left brick->line
                if (point.x < brick->p_min.x && point.x + ball.radius >= brick->p_min.x &&
                    brick->p_min.y < point.y && point.y < brick->p_max.y) {
                    point.x += ball.radius;
                    normal = Vect(-1, 0);
                    break;
                }
                // check with up
                if (point.y < brick->p_min.y && point.y + ball.radius >= brick->p_min.y &&
                    brick->p_min.x < point.x && point.x < brick->p_max.x) {
                    point.y += ball.radius;
                    normal = Vect(0, -1);
                    break;
                }
                // check with right
                if (point.x > brick->p_max.x && point.x - ball.radius <= brick->p_max.x &&
                    brick->p_min.y < point.y && point.y < brick->p_max.y) {
                    point.x -= ball.radius;
                    normal = Vect(1, 0);
                    break;
                }
                // check with down
                if (point.y > brick->p_max.y && point.y - ball.radius <= brick->p_max.y &&
                    brick->p_min.x < point.x && point.x < brick->p_max.x) {
                    point.y -= ball.radius;
                    normal = Vect(0, 1);
                    break;
                }
                t += 0.01f;
            }

            // check if collision happened
            if (normal != Vect{0.0f}) {
                ball.velocity =
                    ball.velocity -
                    2 * (ball.velocity.x * normal.x + ball.velocity.y * normal.y) * normal;

                game_state.current_score += 10.0f;
                add_debug_hit(debug_data, point, normal);

                brick_to_delete = brick;

                if (brick->type == BrickType::strong) {
                    ++brick->hit_count;
                    if (brick->hit_count == 1) {
                        brick->color = ImColor(255, 255, 0);
                    } else if (brick->hit_count == 2) {
                        brick->color = ImColor(255, 0, 0);
                    } else if (brick->hit_count > 3) {
                        delete_brick = true;
                    }
                } else if (brick->type == BrickType::double_ball) {
                    balls_list.push_back(Ball{ball.position, ball.velocity * -1.0f, ball.radius,
                                              ball.initial_speed});
                    delete_brick = true;
                } else if (brick->type == BrickType::move_down) {
                    is_moving_down = true;
                    delete_brick = true;
                } else {
                    delete_brick = true;
                }
            }

            if (brick->p_max.y > carriage.p_min.y) {
                game_state.is_over = true;
            }
        }

        if (delete_brick) {
            bricks_list.erase(brick_to_delete);
        }

        if (bricks_list.empty()) {
            game_state.is_over = true;
        }
    }
}

void ArkanoidImpl::update_carriage(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) {
    float move_speed = 500.0f * elapsed;

    if (io.KeysDown[GLFW_KEY_A] || io.KeysDown[GLFW_KEY_LEFT]) {
        carriage.p_min.x -= move_speed;
        carriage.p_max.x -= move_speed;
    }
    if (io.KeysDown[GLFW_KEY_D] || io.KeysDown[GLFW_KEY_RIGHT]) {
        carriage.p_min.x += move_speed;
        carriage.p_max.x += move_speed;
    }

    if (carriage.p_min.x < 0) {
        float overflow = -carriage.p_min.x;
        carriage.p_min.x += overflow;
        carriage.p_max.x += overflow;
    } else if (carriage.p_max.x > world_size.x) {
        float overflow = carriage.p_max.x - world_size.x;
        carriage.p_min.x -= overflow;
        carriage.p_max.x -= overflow;
    }

    for (auto& ball : balls_list) {
        Vect ball_next_pos = ball.position + ball.velocity * elapsed;

        float t = 0.0f;
        Vect collision_point;
        Vect normal;

        while (t < 1.0f) {
            Vect point = ball.position * (1.0f - t) + ball_next_pos * t;

            if (point.y >= carriage.p_min.y - ball.radius &&
                point.y <= carriage.p_max.y + ball.radius &&
                point.x >= carriage.p_min.x - ball.radius &&
                point.x <= carriage.p_max.x + ball.radius) {

                if (point.y < carriage.p_min.y) {
                    normal = Vect(0.0f, -1.0f);
                    collision_point = Vect(point.x, carriage.p_min.y - ball.radius);
                } else {
                    float dx_left = std::abs(point.x - carriage.p_min.x);
                    float dx_right = std::abs(point.x - carriage.p_max.x);
                    normal = Vect((dx_left < dx_right) ? -1.0f : 1.0f, 0.0f);
                    collision_point = (normal.x > 0)
                                          ? Vect(carriage.p_max.x + ball.radius, point.y)
                                          : Vect(carriage.p_min.x - ball.radius, point.y);
                }
                float speed = ball.velocity.Length();
                ball.position = collision_point;

                float dot = ball.velocity.x * normal.x + ball.velocity.y * normal.y;
                ball.velocity = ball.velocity - normal * (2.0f * dot);

                float hit_pos = (point.x - carriage.p_min.x) / carriage.width;
                float angle_mod = (hit_pos - 0.5f) * 2.0f;
                ball.velocity.x += angle_mod * 150.0f;

                ball.velocity.Normalize();
                ball.velocity *= ball.initial_speed;

                ball.velocity = ball.velocity.Normalized() * speed;
                add_debug_hit(debug_data, collision_point, normal);
                break;
            }

            t += 0.001f;
        }
    }
}
