//
// Created by Alex on 04/08/2020.
//

#ifndef CPPBOT_STRATEGY_H
#define CPPBOT_STRATEGY_H

#include <memory>
#include "../maneuvers/Maneuver.h"
#include "Targeting.h"

class Strategy {
    GameInfo& info;
public:
    std::unique_ptr<Targeting> targeting;

    Strategy(GameInfo& info);
    std::shared_ptr<Maneuver> nextManeuver();
    int findBoost(bool only_behind_ball = true, std::unique_ptr<vec3> close_point = nullptr);
};


#endif //CPPBOT_STRATEGY_H
