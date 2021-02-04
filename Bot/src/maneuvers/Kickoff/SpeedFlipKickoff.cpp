//
// Created by Alex on 03/08/2020.
//

#include "SpeedFlipKickoff.h"
#include "../../utils/vector_math.h"
#include "../../utils/misc.h"

float DIAGONAL_FLIP_START = 250.0f;
float OC_FLIP_START = 500.0f;
float DIAGONAL_X = 1788.0f;
float DIAGONAL_Y = 2300.0f;
float OC_X = 25.0f;
float OC_Y = 3085.0f;
float CLOSE_Y = 90.0f;
float DIAGONAL_ANGLE = 0.1f;
float OC_ANGLE = 0.14f;
float BALL_DODGE_DISTANCE = 700.0f;
float BALL_DODGE_DURATION = 0.175f;
float BALL_DODGE_DELAY = 0.2f;
float DISTANCE_DELTA = 250.0f;

void SpeedFlipKickoff::step() {
    if (not ready) setup();

    if (diagonal) {

        switch (fase) {
            case 1:
                drive.target = vec3{x, y, 0};
                drive.step(info.dt);
                controls = drive.controls;
                controls.boost = true;
                if (norm(info.myCar.position - drive.target) < DIAGONAL_FLIP_START) {
                    fase++;
                    drive.finished = true;
                }
                break;

            case 2:
                if (speedFlip(dir, true)) {
                    fase++;
                    drive.finished = false;
                }
                break;

            case 3:
                dodgeIntoBall();
        }
    } else if (!diagonal) {

        switch (fase) {
            case 1:
                drive.target = vec3{x, y, 0};
                drive.step(info.dt);
                controls = drive.controls;
                controls.boost = true;
                if (norm(info.myCar.position - drive.target) < OC_FLIP_START) {
                    fase++;
                    drive.finished = true;
                }
                break;

            case 2:
                if (speedFlip(dir, false)) {
                    fase++;
                    drive.finished = false;
                }
                break;
            case 3:
                dodgeIntoBall();
        }
    }

    if (!info.game.kickoff_pause) {
        resume_time += info.dt;
        if (resume_time >= 1) finished = true;
    }
}

bool SpeedFlipKickoff::speedFlip(int direction, bool turn) {
    controls = Input();
    float elapsed = 0;
    controls.throttle = 1;
    controls.boost = true;
    if (sf_start == -1) {
        sf_start = info.game.time;
    } else {
        elapsed = info.game.time - sf_start;
        if (!turn and elapsed < 0.065) elapsed += 0.065;
    }
    if (elapsed < 0.065) {
        controls.handbrake = true;
        controls.steer = -1 * direction;
    } else if (elapsed < 0.15) {
        controls.jump = true;
    } else if (elapsed < 0.20) {
        controls.jump = false;
        controls.pitch = -1;
        controls.yaw = direction;
    } else if (elapsed < 0.25) {
        controls.jump = true;
        controls.pitch = -1;
        controls.yaw = direction;
    } else if (elapsed < 0.85) {
        controls.pitch = 1;
        controls.yaw = direction * 0.2;
        controls.jump = false;
    } else if (elapsed < 1.20) {
        controls.roll = direction;
        controls.handbrake = true;
        controls.pitch = 1;
        controls.yaw = direction * 0.70;
    } else if (!info.myCar.on_ground) {
        reorient.target_orientation = look_at(vec3{0, y_close, 0});
        reorient.step(info.dt);
        controls = reorient.controls;
        controls.throttle = 1;
        controls.handbrake = 1;
    } else return true;
    return false;

}

void SpeedFlipKickoff::render(Renderer &renderer) {
}

void SpeedFlipKickoff::dodgeIntoBall() {
    if (not dodging) {
        drive.target = vec3{0, y_close, 0};
        drive.step(info.dt);
        controls = drive.controls;
        controls.boost = false;
        controls.throttle = 1;
        if
                ((
                        (angle_to(info.myCar, drive.target) < DIAGONAL_ANGLE and diagonal)
                        or (angle_to(info.myCar, drive.target) < OC_ANGLE and !diagonal)
                )
                and norm(info.myCar.position - drive.target) < BALL_DODGE_DISTANCE
                and shouldDodge()
                ) {
            dodging = true;
        }
    } else {
        dodge.jump_duration = BALL_DODGE_DURATION;
        dodge.delay = BALL_DODGE_DELAY;
        dodge.direction = direction(info.myCar.position, vec3{0, 0, 92});
        dodge.preorientation = look_at(direction(info.myCar.position, vec3{0, 0, 92}));
        dodge.step(info.dt);
        controls = dodge.controls;
        finished = dodge.finished;
    }
}

void SpeedFlipKickoff::setup() {
    if (diagonal) {
        x = DIAGONAL_X * static_cast<float>(xside);
        y = DIAGONAL_Y * side(info.myCar);
    } else {
        x = OC_X * static_cast<float>(xside);
        y = OC_Y * side(info.myCar);
    }
    y_close = CLOSE_Y * side(info.myCar);

    if (xside * side(info.myCar) < 0) dir = -1;
    ready = true;
}

bool SpeedFlipKickoff::shouldDodge() {
    Car *closest_opponent = closestOpponentToPoint(info, info.ball.position);
    if (closest_opponent == nullptr) return true;
    return norm(info.ball.position - info.myCar.position) + DISTANCE_DELTA >=
           norm(info.ball.position - closest_opponent->position);
}
