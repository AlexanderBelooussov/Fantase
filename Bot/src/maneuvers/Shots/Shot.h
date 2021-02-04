//
// Created by Alex on 06/08/2020.
//

#ifndef CPPBOT_SHOT_H
#define CPPBOT_SHOT_H

#include <mechanics/drive.h>
#include "../../utils/game_info.h"
#include "../../utils/Prediction.h"
#include "../Maneuver.h"
#include "../MoveTo.h"
#include "../../strategy/Targeting.h"

/**
 * Base shot Maneuver
 */
class Shot : public Maneuver {
public:
    float last_update_time;
    /**
     * Whether the prediction can look for the bot to move backwards
     */
    bool allow_backwards = false;
    /**
     * Whether we drive backwards to take the shot
     */
    bool backwards = false;
    Prediction prediction;
    /**
     * Targeting class for recalculating the shot direction during the approach
     */
    Targeting targeting;
    MoveTo move;
    vec3 target;
    float remaining_time = 9999;

    Shot(GameInfo &info);

    /**
     * Update the prediction during the shot
     * @param cond: Condition that the prediction slice must satisfy
     */
    virtual void updatePrediction(const std::function<bool(Car & , Ball & )> &cond);

    virtual void step();

    /**
     * Look for a valid prediction slice
     * Used before choosing to go for a shot
     */
    virtual void initialPrediction();

    /**
     * How the bot approiaches the shot
     * @param shot_direction: Vector of the direction we want to shoot at
     * @param desired_position: The position we want to be before hitting the ball
     */
    virtual void approachShot(vec3 shot_direction, vec3 desired_position) = 0;
};

/**
 * Sets a condition that needs to be fulfilled for a prediction slice to be considered viable
 * @param car: RLU car object
 * @param ball: RLU ball object
 * @return Whether the condition has been satisfied
 */
bool shotPredictionCondition(Car &car, Ball &ball);

#endif //CPPBOT_SHOT_H
