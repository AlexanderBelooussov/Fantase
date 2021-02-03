//
// Created by Alex on 06/08/2020.
//

#ifndef CPPBOT_PREDICTION_H
#define CPPBOT_PREDICTION_H


#include "game_info.h"
#include <functional>

class Prediction {
    float estimate_max_car_speed();
    float estimate_time(vec3 target, float speed, int dd=1);
public:
    Car car;
    Ball ball;

    Prediction();

    Prediction(Car car, Ball ball, const std::function<bool(Car&, Ball&)>& condition, bool backwards = false);

    bool is_viable;
    float time;
    vec3 ball_position;
};


#endif //CPPBOT_PREDICTION_H
