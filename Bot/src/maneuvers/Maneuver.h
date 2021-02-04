//
// Created by Alex on 01/08/2020.
//

#ifndef CPPBOT_MANEUVER_H
#define CPPBOT_MANEUVER_H

#include "simulation/input.h"
#include "rlbot/renderer.h"
#include "../utils/game_info.h"

class Maneuver {
    /**
     * Base maneuver class
     */

public:
    GameInfo & info;
    Input controls;
    bool finished = false;
    bool interuptable = true;

    Maneuver(GameInfo & info) : info(info) {};

    /**
     * This function will be called every tick by the bot
     * Sets the controls
     */
    virtual void step() = 0;

    /**
     * This function gets called every tick and is used for debugging
     * @param renderer
     */
    virtual void render(Renderer & renderer) {};
};


#endif //CPPBOT_MANEUVER_H
