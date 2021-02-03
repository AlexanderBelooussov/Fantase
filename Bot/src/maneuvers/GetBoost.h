//
// Created by Alex on 28/10/2020.
//

#ifndef CPPBOT_GETBOOST_H
#define CPPBOT_GETBOOST_H


#include "MoveTo.h"

class GetBoost: public MoveTo {
public:
    int boost = -1;

    GetBoost(GameInfo& info);
    virtual void step();
    virtual void render(Renderer & renderer);
};


#endif //CPPBOT_GETBOOST_H
