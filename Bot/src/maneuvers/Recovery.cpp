//
// Created by Alex on 02/08/2020.
//

#include "Recovery.h"

Recovery::Recovery(GameInfo &info) : Maneuver(info), reorient(info.myCar), target(vec3{0,0,0}) {

}

Recovery::Recovery(GameInfo &info, vec3 target) : Maneuver(info), reorient(info.myCar), target(target){

}

void Recovery::step() {
    reorient.target_orientation = look_at(xy(info.myCar.velocity));
    reorient.step(info.dt);
    controls = reorient.controls;
    controls.handbrake = true;
    controls.throttle = 1;
    finished = info.myCar.on_ground;
}
