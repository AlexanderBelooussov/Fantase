//
// Created by Alex on 04/08/2020.
//

#include "FrontFlipKickoff.h"
#include "../../utils/vector_math.h"

float VELOCITY_TO_DODGE = 1600.0f;
float DODE_DELAY = 0.2f;
float DODGE_JUMP_DURATION = 0.22f;
float DISTANCE_TO_DODGE = 800.0f;

void FrontFlipKickoff::step() {
    switch (fase){
        case 1:
            drive.target = vec3{0, 0, 0};
            drive.step(info.dt);
            controls = drive.controls;
            controls.boost = true;
            if (norm(info.myCar.velocity) > VELOCITY_TO_DODGE) {
                fase++;
                drive.finished = true;
            }
            break;

        case 2:
            dodge.jump_duration = 0.1f;
            //dodge.delay = 0.1f;
            dodge.direction = direction(info.myCar.position, vec3{0,0,92});
            dodge.preorientation = look_at(direction(info.myCar.position, vec3{0,0,92}));
            dodge.postorientation = look_at(direction(info.myCar.position, vec3{0,0,92}));
            dodge.step(info.dt);
            controls = dodge.controls;
            if (abs(info.myCar.forward()[2]) < 0.1 and angle_to(info.myCar, vec3{0,0,92}) < 1 and not info.myCar.supersonic) controls.boost = true;
            if (dodge.finished and info.myCar.on_ground){
                fase++;
                drive.finished = false;
            }
            break;

        case 3:

                drive.target = vec3{0, 0, 0};
                drive.step(info.dt);
                controls = drive.controls;
                controls.boost = not info.myCar.supersonic;
                controls.throttle = 1;
                if (norm(info.myCar.position - drive.target) < DISTANCE_TO_DODGE) {
                    drive.finished = true;
                    fase++;
                    dodge.timer = 0;
                    dodge.finished = false;
                }
            break;
        case 4:
                dodge.jump_duration = DODGE_JUMP_DURATION;
                dodge.delay = DODE_DELAY;
                dodge.direction = direction(info.myCar.position, vec3{0,0,92});
                dodge.preorientation = look_at(direction(info.myCar.position, vec3{0,0,92}));
                dodge.step(info.dt);
                controls = dodge.controls;
                finished = dodge.finished;
            break;
    }
}

FrontFlipKickoff::FrontFlipKickoff(GameInfo &info) : Maneuver(info), dodge(info.myCar), reorient(info.myCar), drive(info.myCar) {
    drive.speed = 2300;
    interuptable = false;
}
