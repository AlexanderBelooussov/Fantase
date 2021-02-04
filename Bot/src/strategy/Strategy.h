//
// Created by Alex on 04/08/2020.
//

#ifndef CPPBOT_STRATEGY_H
#define CPPBOT_STRATEGY_H

#include <memory>
#include "../maneuvers/Maneuver.h"
#include "Targeting.h"
/**
 * Base strategy class
 * This should only really have basic shot seeking capabilities
 * More in depth strategy should be implemented in its own class
 */
class Strategy {
    GameInfo& info;
public:
    std::unique_ptr<Targeting> targeting;

    Strategy(GameInfo& info);

    /**
     * Finds a suitable Maneuver to perform
     * @return Pointer to a Maneuver
     */
    std::shared_ptr<Maneuver> nextManeuver();

    /**
     * Finds a suitable boost pad to cllect
     * @param only_behind_ball: True if it should only consider boosts that are behind the ball
     * @param close_point: Point that the boost should be closest to, should we have multiple options
     * @return Index of the boost pad
     */
    int findBoost(bool only_behind_ball = true, std::unique_ptr<vec3> close_point = nullptr);
};


#endif //CPPBOT_STRATEGY_H
