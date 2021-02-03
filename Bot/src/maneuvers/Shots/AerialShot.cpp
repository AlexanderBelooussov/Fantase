//
// Created by Alex on 30/10/2020.
//

#include "AerialShot.h"
#include "../../utils/misc.h"
#include "../../utils/vector_math.h"
#include "../../utils/vector_math.h"

float MIN_TAKE_OFF_ANGLE = 15.f;

AerialShot::AerialShot(GameInfo &info) : Shot(info), aerial(info.myCar) {

}

void AerialShot::step() {
    //if (not aerialing and remaining_time > 0.5 and info.time - last_update_time > 0.2) {
    if (info.time - last_update_time > 0.2) {
        std::function<bool(Car &, Ball &)> cond = aerialShotPredictionCondition;
        updatePrediction(cond);
        last_update_time = info.time;
        if (finished) return;
    }
    if (not aerialing) {
        vec3 local_to_ball = local(info.myCar, prediction.ball_position);
        float vertical = local_to_ball[2];
        float horizontal = norm(ground(local_to_ball));
        float angle = angle_between(vec3{1, 0, 0}, ground(local_to_ball));
        float ratio = vertical / horizontal;
        vec3 shot_direction = normalize(direction(info.myCar.position, prediction.ball_position));
        vec3 desired_position = prediction.ball_position - shot_direction * 50;
        approachShot(shot_direction, desired_position);
        if (ratio > degreeToRadian(MIN_TAKE_OFF_ANGLE)) aerialing = true;
    }
    else if (aerialing){
        aerial.arrival_time = prediction.time;
        aerial.double_jump = prediction.ball_position[2] > 800;
        //std::cout << info.myCar.name << " doing aerial to " << prediction.ball_position << " in " << prediction.time - info.time << " seconds" << std::endl;
        aerial.up = vec3{0,0,1};
        aerial.target_position = prediction.ball_position;
        aerial.step(info.dt);
        controls = aerial.controls;
        finished = aerial.finished;
    }
    remaining_time = prediction.time - info.time;
}

void AerialShot::render(Renderer &renderer) {
    renderer.DrawSphere(Color::cyan(), sphere{prediction.ball_position, 91.25f});
    renderer.DrawLine3D(Color::white(), info.myCar.position, prediction.ball_position);
}

void AerialShot::initialPrediction() {
    std::function<bool(Car &, Ball &)> cond = aerialShotPredictionCondition;
    updatePrediction(cond);
    last_update_time = info.time;
}

void AerialShot::updatePrediction(const std::function<bool(Car &, Ball &)> &cond) {
    Shot::updatePrediction(cond);
}

bool aerialShotPredictionCondition(Car& car, Ball& ball){
    if (ball.position[2] < 300)
        return false;
    Aerial a {car};
    a.target_position = ball.position;
    a.arrival_time = ball.time;
    a.up = vec3{0,0,1};
    a.double_jump = true;
    bool go = a.is_viable();
    //def range_map(x, in_min, in_max, out_min, out_max):
    //return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
    //float time = (ball.position[2] - 300) * (2.5 - 0.8) / (1800 - 300) + 0.8;
    return go and ball.time - car.time > 0.8;
}

void AerialShot::approachShot(vec3 shot_direction, vec3 desired_position) {
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
