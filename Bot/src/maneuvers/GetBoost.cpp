//
// Created by Alex on 28/10/2020.
//

#include "GetBoost.h"
#include "../utils/misc.h"
#include "../utils/vector_math.h"

GetBoost::GetBoost(GameInfo &info) : MoveTo(info) {
}

void GetBoost::step() {
    if (boost == -1) {
        finished = true;
        return;
    }
    target = info.game.pads.at(boost).position;
    speed = 2300;
    MoveTo::step();
    if (not info.game.pads.at(boost).is_active) finished = true;

}

void GetBoost::render(Renderer &renderer) {
    obb box;
    box.center = info.game.pads[boost].position;
    box.half_width = vec3{80, 80, 80};
    box.orientation = mat3{{1,0,0},{0,1,0}, {0,0,1}};
    renderer.DrawOBB(Color::green(), box);
    renderer.DrawLine3D(Color::white(), info.myCar.position, target);
}
