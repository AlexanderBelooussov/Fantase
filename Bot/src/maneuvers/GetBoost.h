//
// Created by Alex on 28/10/2020.
//

#ifndef CPPBOT_GETBOOST_H
#define CPPBOT_GETBOOST_H


#include "MoveTo.h"

/**
 * Maneuver for getting boost
 */
class GetBoost : public MoveTo {
public:
    /**
     * Index of the boost
     */
    int boost = -1;

    GetBoost(GameInfo &info);

    /**
     * Finishes ones the targeted boost becomes inactive
     */
    virtual void step();

    virtual void render(Renderer &renderer);
};


#endif //CPPBOT_GETBOOST_H
