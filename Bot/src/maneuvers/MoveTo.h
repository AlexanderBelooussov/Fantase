//
// Created by Alex on 01/08/2020.
//

#ifndef CPPBOT_MOVETO_H
#define CPPBOT_MOVETO_H


#include "mechanics/drive.h"
#include "Recovery.h"

class MoveTo: public Maneuver {
    Drive drive;
    Recovery recovery;
    bool recovering = false;



public:
    vec3 target;
    float speed = 2300;


    MoveTo(GameInfo& info);

     virtual void step();
    virtual void render(Renderer & renderer);
};


#endif //CPPBOT_MOVETO_H
