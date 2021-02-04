//
// Created by Alex on 02/08/2020.
//

#ifndef CPPBOT_RECOVERY_H
#define CPPBOT_RECOVERY_H

#include "mechanics/reorient.h"
#include "simulation/car.h"
#include "simulation/input.h"
#include "Maneuver.h"

/**
 * Maneuver for landing on wheels
 */
class Recovery : public Maneuver {
    Reorient reorient;
public:
    vec3 target;

    Recovery(GameInfo &info);

    Recovery(GameInfo &info, vec3 target);

    void step();
};


#endif //CPPBOT_RECOVERY_H
