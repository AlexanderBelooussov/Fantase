//
// Created by Alex on 08/08/2020.
//

#ifndef CPPBOT_DODGESHOT_H
#define CPPBOT_DODGESHOT_H

#include <experimental/navigator.h>
#include <experimental/follow_path.h>
#include "Shot.h"

class DodgeShot : public Shot{
    float remaining_time = 99999;
    bool dodging = false;

    float last_touch;

    vec3 desired_position;
    vec3 shot_direction;

    Dodge dodge;

    Navigator navigator;
    FollowPath followpath;
    bool prepared = false;

    void preparePath();

    float calculateJumpTime(float height, float gravity = 650);

    void approachShot(vec3 shot_direction, vec3 desired_position);

public:

    DodgeShot(GameInfo &info);

    void updatePrediction(const std::function<bool(Car&, Ball&)>& cond) override;

    void step() override;

    virtual void render(Renderer & renderer);

    void initialPrediction() override;
};

bool dodgeShotPredictionCondition(Car& car, Ball& ball);


#endif //CPPBOT_DODGESHOT_H
