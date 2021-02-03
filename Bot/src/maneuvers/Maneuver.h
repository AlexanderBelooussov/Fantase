//
// Created by Alex on 01/08/2020.
//

#ifndef CPPBOT_MANEUVER_H
#define CPPBOT_MANEUVER_H

#include "simulation/input.h"
#include "rlbot/renderer.h"
#include "../utils/game_info.h"

class Maneuver {

public:
    GameInfo & info;
    Input controls;
    bool finished = false;
    bool interuptable = true;

    Maneuver(GameInfo & info) : info(info) {};

    virtual void step() = 0;

    virtual void render(Renderer & renderer) {};
};


#endif //CPPBOT_MANEUVER_H
