//
// Created by Alex on 06/08/2020.
//

#include "Prediction.h"
#include "vector_math.h"
#include <cmath>
#include "misc.h"

Prediction::Prediction(Car car, Ball ball, const std::function<bool(Car&, Ball&)>& condition, bool backwards) : car(car), ball(ball) {
    // find the first reachable ball slice that also meets the predicate
    float speed = 1000;
    if (!backwards) speed = estimate_max_car_speed();

    is_viable = false;
    for (int i = 0; i < 120 * 6; i ++){
        ball.step(1.f/120.f);
        float time;
        if (backwards)
            time = estimate_time(ball.position, speed, -1);
        else
            time = estimate_time(ball.position, speed, 1);

        if (time < ball.time - car.time and (!condition or condition(car, ball))){
            is_viable = true;
            break;
        }
    }

    time = ball.time;
    ball_position = ball.position;
}

float Prediction::estimate_max_car_speed(){
    return clamp(std::max(norm(car.velocity), 1300.f) + car.boost * 100.f, 1600.f, 2300.f);
}

float Prediction::estimate_time(vec3 target, float speed, int dd){
    float travel = (distance(car.position, target)) / speed;
    float turning = angle_between(car.forward() * dd, direction(car.position, target)) / 3.14159265358979323846 * 2; //M_PI is being weird
    if (turning < 1){
        pow(turning,  2);
    }

    float acceleration = (speed * dd - dot(car.velocity, car.forward())) / 2100 * 0.2 * dd / std::max(car.boost / 20, 1);
    return travel + acceleration + turning * 0.7;
}

Prediction::Prediction() {}
