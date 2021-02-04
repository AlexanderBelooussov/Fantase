//
// Created by Alex on 04/08/2020.
//

#ifndef CPPBOT_FRONTFLIPKICKOFF_H
#define CPPBOT_FRONTFLIPKICKOFF_H


#include <mechanics/reorient.h>
#include <mechanics/drive.h>
#include <mechanics/dodge.h>
#include "../Maneuver.h"
/**
 * Basic Kickoff using front flips
 * Used for central spawn kickoffs
 */
class FrontFlipKickoff: public Maneuver {
    Drive drive;
    Reorient reorient;
    Dodge dodge;
    int fase = 1;
public:
    FrontFlipKickoff(GameInfo &info);

    void step() override;
};


#endif //CPPBOT_FRONTFLIPKICKOFF_H
