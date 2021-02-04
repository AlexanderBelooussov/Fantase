//
// Created by Alex on 03/08/2020.
//

#ifndef CPPBOT_GROUNDSHOT_H
#define CPPBOT_GROUNDSHOT_H



#include "Shot.h"

/**
 * Shot Maneuver for hitting rolling balls
 * @todo currently very much broken
 */
class GroundShot : public Shot{
    vec3 desired_position;
    float last_touch;
    vec3 shot_direction;
    vec3 far_point;
    vec3 projection;
    vec3 ball_to_car;
    float remaining_time = 9999;

public:

    GroundShot(GameInfo &info);

    void updatePrediction(const std::function<bool(Car&, Ball&)>& cond) override;

    void step() override;

    virtual void render(Renderer & renderer);

    void initialPrediction() override;

    void approachShot(vec3 shot_direction, vec3 desired_position) override {};
};

bool groundShotPredictionCondition(Car& car, Ball& ball);

#endif //CPPBOT_GROUNDSHOT_H
