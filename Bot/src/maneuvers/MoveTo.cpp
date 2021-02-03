//
// Created by Alex on 01/08/2020.
//

#include "MoveTo.h"
#include <iostream>
#include "../utils/vector_math.h"

MoveTo::MoveTo(GameInfo& info): Maneuver(info), drive(info.myCar), recovery(info) {}

void MoveTo::step() {
    //drive
    if (not recovering and not info.myCar.on_ground) {
        recovering = true;
    }
    if (not recovering){

        drive.target = target;
        drive.speed = speed;
        drive.step(info.dt);

        // powerslide
        float angleToTarget = angle_to(info.myCar, target);
        controls.handbrake = (angleToTarget > 1.5f && info.myCar.position[2] < 300);
        controls = drive.controls;
        finished = drive.finished;
    }

    //in the air -> recover
    if (recovering) {
        recovery.target = target;
        recovery.step();
        controls = recovery.controls;
        recovering = not recovery.finished;
    }

    finished = true;
}

void MoveTo::render(Renderer &renderer) {
    Color c = Color::blue();
    if (info.myCar.team == 1) c = Color::red();
    obb box = info.myCar.hitbox();
    box.center = dot(info.myCar.orientation, info.myCar.hitbox_offset) + target;
    renderer.DrawOBB(c, box);
    renderer.DrawLine3D(Color::white(), info.myCar.position, target);
}
