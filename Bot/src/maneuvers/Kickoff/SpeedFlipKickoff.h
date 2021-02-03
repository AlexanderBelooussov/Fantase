//
// Created by Alex on 03/08/2020.
//

#ifndef CPPBOT_SPEEDFLIPKICKOFF_H
#define CPPBOT_SPEEDFLIPKICKOFF_H


#include <mechanics/drive.h>
#include <mechanics/dodge.h>
#include "../Maneuver.h"


class SpeedFlipKickoff : public Maneuver {
    int fase = 1;
    Drive drive;
    Dodge dodge;
    Reorient reorient;
    float sf_start = -1;
    int dir = 1;
    float x, y, y_close;
    bool dodging = false;
    bool ready = false;
    float resume_time = 0;

    void setup();

    bool shouldDodge();
public:
    bool diagonal = true;
    int xside = 1;

    SpeedFlipKickoff(GameInfo &info, bool diagonal, int xside) : Maneuver(info), drive(info.myCar), dodge(info.myCar), reorient(info.myCar), diagonal(diagonal), xside(xside) {
        drive.speed = 2300;
        interuptable = false;

    };

    void step() override;

    void render(Renderer &renderer) override;

    bool speedFlip(int direction, bool turn = true);

    void dodgeIntoBall();

};


#endif //CPPBOT_SPEEDFLIPKICKOFF_H
