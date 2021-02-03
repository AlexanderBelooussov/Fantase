//
// Created by Alex on 30/10/2020.
//

#ifndef CPPBOT_AERIALSHOT_H
#define CPPBOT_AERIALSHOT_H


#include <mechanics/aerial.h>
#include "Shot.h"

class AerialShot: public Shot {
    Aerial aerial;
public:
    bool aerialing = false;
    AerialShot(GameInfo &info);

    void updatePrediction(const std::function<bool(Car&, Ball&)>& cond) override;

    void step() override;

    virtual void render(Renderer & renderer);

    void initialPrediction() override;

    void approachShot(vec3 shot_direction, vec3 desired_position) override;
};

bool aerialShotPredictionCondition(Car& car, Ball& ball);


#endif //CPPBOT_AERIALSHOT_H
