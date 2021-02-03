//
// Created by Alex on 03/08/2020.
//

#include "GroundShot.h"
#include "../../utils/vector_math.h"
#include "../../utils/misc.h"

GroundShot::GroundShot(GameInfo &info) : Shot(info) {
    last_touch = info.game.latest_touch_time;
}

void GroundShot::updatePrediction(const std::function<bool(Car&, Ball&)>& cond) {
    Shot::updatePrediction(cond);
}

void GroundShot::step() {
    //update prediction
   if (remaining_time > 0.5) {
        std::function<bool(Car &, Ball &)> cond = groundShotPredictionCondition;
        updatePrediction(cond);
    }
    remaining_time = prediction.time - info.time;

//    // calc desired car position
    vec3 ball_to_target = flatten(target - prediction.ball_position);
    shot_direction = normalize(ball_to_target - flatten(prediction.ball.velocity));
    desired_position = flatten(prediction.ball_position) - shot_direction * (93+64);

    // go to that position, but face the correct direction too
    // car location if ball is origin
    ball_to_car = flatten(info.myCar.position - prediction.ball_position);
    // project that car onto shot vector
    vec3 on_shot_line = project(ball_to_car, -shot_direction);
    projection = on_shot_line-ball_to_car;
    far_point = info.myCar.position + projection;
    move.target = (prediction.ball_position + far_point)/2.f;
    move.speed = distance(info.myCar.position, desired_position) / remaining_time;
    move.step();
    controls = move.controls;

//    float speed = distance(info.myCar.position, desired_position) / remaining_time;
//    navigator.analyze_surroundings(remaining_time);
//    followpath.path = navigator.path_to(desired_position, shot_direction, speed);
//    followpath.step(info.dt);
//    controls = followpath.controls;


    //finish on any contact
    float angle_to_goal = angle_between((info.myCar.position - prediction.ball_position), (info.theirGoal - prediction.ball_position));
    finished = info.game.latest_touch_time > last_touch or angle_to_goal < 3.1415/2.f;
}


void GroundShot::render(Renderer &renderer) {
    Color c = Color::blue();
    if (info.myCar.team == 1) c = Color::red();
    obb box;
    box.center = desired_position;
    box.half_width = vec3{10, 10, 10};
    box.orientation = mat3{{1,0,0},{0,1,0}, {0,0,1}};
    renderer.DrawOBB(c, box);
    renderer.DrawSphere(Color::white(), sphere{prediction.ball_position, 91.25f});
    renderer.DrawLine3D(Color::white(), prediction.ball_position, prediction.ball_position + shot_direction * 250);

    renderer.DrawLine3D(Color::white(), info.myCar.position, far_point);
    renderer.DrawLine3D(Color::white(), info.myCar.position, move.target);
    renderer.DrawLine3D(Color::white(), prediction.ball_position, far_point);

    renderer.DrawLine3D(Color::red(), vec3{0,0,0}, flatten(prediction.ball.velocity));
    renderer.DrawLine3D(Color::blue(), vec3{0,0,0}, shot_direction);
    renderer.DrawLine3D(Color::white(), vec3{0,0,0}, target - prediction.ball_position);
    //renderer.DrawLine3D(Color::white(), vec3{0,0,0}, shot_direction * -8000);


}

void GroundShot::initialPrediction() {
    std::function<bool(Car&, Ball&)> cond = groundShotPredictionCondition;
    updatePrediction(cond);
    last_update_time = info.time;
}

bool groundShotPredictionCondition(Car &car, Ball &ball) {
    vec3 theirGoal {0.f, -5120.f * side(car), 0.f};
    float angle_to_goal = angle_between((flatten(car.position) - flatten(ball.position)), (theirGoal - flatten(ball.position)));
    if (angle_to_goal > 3.1415/2) {
        return ball.position[2] < 100
            and angle_between(ball.velocity, (car.position - ball.position)) < 0.5;
    }
    return false;
}