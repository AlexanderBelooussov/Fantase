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

class Shot : public Maneuver{
public:
    float last_update_time;
    bool allow_backwards = false;
    bool backwards = false;
    Prediction prediction;
    Targeting targeting;
    MoveTo move;
    vec3 target;
    float remaining_time = 9999;

    Shot(GameInfo &info);

    virtual void updatePrediction(const std::function<bool(Car&, Ball&)>& cond);

    virtual void step();

    virtual void initialPrediction();

    virtual void approachShot(vec3 shot_direction, vec3 desired_position)=0;
};

bool shotPredictionCondition(Car& car, Ball& ball);

#endif //CPPBOT_SHOT_H
