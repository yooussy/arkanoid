struct Ball {
    Vect position{0.0f};
    Vect next_position{0.0f};
    Vect velocity{0.0f};

    float radius = 0.0f;
    float initial_speed = 0.0f;

    Ball(Vect pos, Vect vel, float rad, float init_speed)
        : position(pos), velocity(vel), radius(rad), initial_speed(init_speed) {
    }
};
