//
// Created by Alex on 03/08/2020.
//

#ifndef CPPBOT_MISC_H
#define CPPBOT_MISC_H

#include <simulation/car.h>
#include "game_info.h"

int side(Car& car);

Car* closestOpponentToPoint(GameInfo& info, vec3 point);

template <typename T>
T clamp(const T v, const T min_value, const T max_value);
#endif //CPPBOT_MISC_H

int sign(const float v);

float degreeToRadian(float degree);