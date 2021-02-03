//
// Created by Alex on 06/08/2020.
//
#include "misc.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
int side(Car& car){
    if (car.team == 1) return 1;
    return -1;
}

Car* closestOpponentToPoint(GameInfo& info, vec3 point){
    Car * result = nullptr;
    float distance = 9999999999.0f;
    for (Car* car : info.opponents){
        float car_dist = norm(point - car->position);
        if (distance > car_dist){
            result = car;
            distance = car_dist;
        }
    }
    return result;
}

template <typename T>
T clamp(const T v, const T min_value, const T max_value) {
    return std::min(max_value, std::max(v, min_value));
}
template float clamp(const float v, const float min_value, const float max_value);
template double clamp(const double v, const double min_value, const double max_value);

int sign(const float v) {
    if (v > 0) return 1;
    else if (v < 0) return -1;
    return 0;
}

float degreeToRadian(float degree){
    return degree * M_PI / 180;
}