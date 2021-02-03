//
// Created by Alex on 08/08/2020.
//

#include "DodgeShot.h"
#include "../../utils/vector_math.h"
#include "../../utils/misc.h"
#include "../../../../RLUtilities/inc/simulation/ball.h"


float JUMP_ACCELERATION = 1458.333374;
float JUMP_INITIAL = 291.667;
float MAX_JUMP_DURATION = 0.2;

DodgeShot::DodgeShot(GameInfo &info) : Shot(info), dodge(info.myCar), navigator(info.myCar), followpath(info.myCar) {
    last_touch = info.game.latest_touch_time;
}

void DodgeShot::updatePrediction(const std::function<bool(Car &, Ball &)> &cond) {
    Shot::updatePrediction(cond);
}

void DodgeShot::step() {
    //update prediction
    //if (remaining_time > 0.5 and info.time - last_update_time > 0.1) {
    if (info.time - last_update_time > 0.1) {
        std::function<bool(Car &, Ball &)> cond = dodgeShotPredictionCondition;
        updatePrediction(cond);
        last_update_time = info.time;
        targeting.generateOpponentCones();
        target = prediction.ball_position + 1000 * targeting.determineShotDirection(prediction.ball_position);
        //prepared = false;
    }
    remaining_time = prediction.time - info.time;

    //shot direction is the direction we want to dodge in
    vec3 ball_to_target = flatten(target - prediction.ball_position);
    //shot_direction = normalize(ball_to_target - normalize(flatten(prediction.ball.velocity)));
    shot_direction = normalize(ball_to_target);

    //position we want to dodge at
    desired_position = prediction.ball_position - shot_direction * (93 + 50);
    //desired_position = prediction.ball_position - shot_direction * (93);

    //calculate jump time
    vec3 local_to_ball = local(info.myCar, prediction.ball_position);
    float jump_time = calculateJumpTime(local_to_ball[2]);



    //approach ball
    if (!dodging) {
        approachShot(shot_direction, desired_position);
        if
        (
                remaining_time < jump_time + 0.05 and
                info.myCar.on_ground and
                abs(angle_between(flatten(info.myCar.position), flatten(desired_position)))< 0.1
        )
            dodging = true;

        finished = info.game.latest_touch_time > last_touch or remaining_time < -1;
        float angle_to_goal = angle_between((flatten(info.myCar.position) - flatten(info.ball.position)),(info.theirGoal - flatten(info.ball.position)));
        if (angle_to_goal <= 3.1415 / 2) finished = true;
        dodge.jump_duration = jump_time;
        dodge.delay = jump_time;
    }

        //dodge into ball
    else if (not finished){
        dodge.preorientation = look_at(direction(info.myCar.position, prediction.ball_position));
        //dodge.direction = xy(shot_direction);
        dodge.direction = xy(direction(info.myCar.position, prediction.ball_position));
        dodge.step(info.dt);
        controls = dodge.controls;
        finished = dodge.finished;
    }

}

void DodgeShot::render(Renderer &renderer) {
    Color c = Color::blue();
    if (info.myCar.team == 1) c = Color::red();
    obb box;
    box.center = desired_position;
    box.half_width = vec3{10, 10, 10};
    box.orientation = mat3{{1,0,0},{0,1,0}, {0,0,1}};
    renderer.DrawOBB(c, box);
    renderer.DrawSphere(Color::white(), sphere{prediction.ball_position, 91.25f});
    renderer.DrawLine3D(Color::green(), desired_position, desired_position + shot_direction * 400);
    renderer.DrawLine3D(c, desired_position, target);
    renderer.DrawLine3D(Color::cyan(), info.ball.position, info.ball.position + shot_direction*1000);

    renderer.DrawLine3D(Color::white(), info.myCar.position, move.target);

    if (followpath.path.length > 0) {
        renderer.DrawPolyLine3D(Color::green(), followpath.path.points);

    }
}

void DodgeShot::initialPrediction() {
    std::function<bool(Car &, Ball &)> cond = dodgeShotPredictionCondition;
    updatePrediction(cond);
    last_update_time = info.time;
}

void DodgeShot::preparePath() {
    navigator.analyze_surroundings(remaining_time - 0.2);
    followpath.path = navigator.path_to(desired_position, shot_direction, 500);
    std::cout << "path length " << followpath.path.length << std::endl;
    if (followpath.path.length < 0 or distance(info.myCar.position, desired_position) > 2 * followpath.path.length) {
        finished = true;
    }
    if (distance(followpath.path.points.back(), desired_position) > 100) {
        finished = true;
    } else {
        followpath.calculate_plan(followpath.path, prediction.time,
                                  500); //distance(info.myCar.position, desired_position)/remaining_time)
        prepared = true;
    }

}

bool dodgeShotPredictionCondition(Car &car, Ball &ball) {
    vec3 theirGoal{0.f, -5120.f * side(car), 0.f};
    float angle_to_goal = angle_between((flatten(car.position) - flatten(ball.position)),
                                        (theirGoal - flatten(ball.position)));
    //if (angle_to_goal > 3.1415 / 2) {
        return ball.position[2] < 300;
    //}
    return false;
}

float DodgeShot::calculateJumpTime(float height, float gravity) {
    float c = 0.5 * JUMP_ACCELERATION-gravity * std::pow(MAX_JUMP_DURATION, 2) + JUMP_INITIAL * MAX_JUMP_DURATION;
    float a_2 = (JUMP_ACCELERATION - gravity) * MAX_JUMP_DURATION + JUMP_INITIAL;
    if (height < c){
        float x = (-JUMP_INITIAL + sqrt(std::pow(JUMP_INITIAL, 2) + 2 * (JUMP_ACCELERATION - gravity) * height))/(JUMP_ACCELERATION-gravity);
        return x;
    }
    else{
        float x = -((-a_2 - MAX_JUMP_DURATION * gravity) + std::sqrt(2 * gravity * (-a_2*MAX_JUMP_DURATION + c - 0.5 * std::pow(MAX_JUMP_DURATION, 2) * gravity-height) + std::pow(a_2 + MAX_JUMP_DURATION * gravity, 2)))/gravity;
        if (x < 0) x = -((-a_2 - MAX_JUMP_DURATION * gravity) - std::sqrt(2 * gravity * (-a_2*MAX_JUMP_DURATION + c - 0.5 * std::pow(MAX_JUMP_DURATION, 2) * gravity-height) + std::pow(a_2 + MAX_JUMP_DURATION * gravity, 2)))/gravity;
        if (x < 0) return 1.2;
        return x;
    }
    /*float timeNeeded;
    if (height <= 74.48){
        timeNeeded = std::max(0.f,sqrt(10100*height+531441)-729) / 2020;
    }
    else{
        timeNeeded = (1458 - sqrt(1888839 - 8125*height)) / 1625;
    }
    if (timeNeeded < 0) {
        finished = true;
        return 0;
    }
    return timeNeeded;*/

}

void DodgeShot::approachShot(vec3 shot_direction, vec3 desired_position) {
    //Gosling utils style approachShot
    vec3 car_to_ball =  info.ball.position - info.myCar.position;
    int side_of_shot = sign(dot(cross(shot_direction, vec3{0, 0, 1}), car_to_ball));

    vec3 car_to_dodge = desired_position - info.myCar.position;
    vec3 car_to_dodge_perp = cross(car_to_dodge, vec3{0, 0, static_cast<float>(side_of_shot)});
    float distance_remaining = norm(xy(car_to_dodge));

    float speed_required = distance_remaining/remaining_time;
    vec3 acceleration_required = backsolve(info.myCar, desired_position, remaining_time, 650);
    vec3 local_acceleration = local_velocity(info.myCar, acceleration_required);
    float adjustment = angle_between(shot_direction, car_to_dodge) * distance_remaining / 2.0;
    adjustment *= clamp(584.0 - acceleration_required[2], 0.0, 584.0) / 584.0;
    vec3 final_target = desired_position + (normalize(car_to_dodge_perp) * adjustment);
    move.target = final_target;
    move.step();
    move.speed = speed_required;
    controls = move.controls;
}
