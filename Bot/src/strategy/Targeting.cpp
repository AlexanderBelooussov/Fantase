//
// Created by Alex on 02/11/2020.
//

#include "Targeting.h"
#include "../utils/vector_math.h"
#include "../utils/misc.h"

#define _USE_MATH_DEFINES

#include <math.h>


Targeting::Targeting(const GameInfo &info) : info(info) {
    for (int i = 0; i < info.game.num_cars; i++) {
        Car &car = info.game.cars[i];
        if (car.team != info.myCar.team) {
            opponent_indices.emplace_back(i);
        }
    }
}

void Targeting::generateOpponentCones() {
    opponent_cones.clear();
    opponent_indices.clear();
    for (int i = 0; i < info.game.num_cars; i++) {
        Car &car = info.game.cars[i];
        if (car.team != info.myCar.team) {
            vec3 first = flatten(car.forward());
            std::tuple <vec3, vec3> cone = getConeDirection(first);
            opponent_cones.emplace_back(first, std::get<0>(cone), std::get<1>(cone));
        }
    }
    for (int i = 0; i < info.game.num_cars; i++) {
        Car &car = info.game.cars[i];
        if (car.team != info.myCar.team) {
            opponent_indices.emplace_back(i);
        }
    }
}

void Targeting::render(Renderer &renderer) {
    if (info.myCar.team == 0) { //only render one team to reduce clutter
        vec3 direction = normalize(flatten(info.ball.position - info.myCar.position));
        std::tuple <vec3, vec3> ball_cone = getConeDirection(direction);
        vec3 ball_position = info.ball.position;
        renderer.DrawLine3D(Color::green(), flatten(ball_position), flatten(ball_position + 4000 * direction));
        renderer.DrawLine3D(Color::green(), ball_position, ball_position + 4000 * std::get<0>(ball_cone));
        renderer.DrawLine3D(Color::green(), ball_position, ball_position + 4000 * std::get<1>(ball_cone));

        int index = 0;
        for (int i = 0; i < info.game.num_cars; i++) {
            Car &car = info.game.cars[i];
            if (car.team != info.myCar.team) {
                std::tuple <vec3, vec3, vec3> tup = opponent_cones[index];
                renderer.DrawLine3D(Color::cyan(), flatten(car.position),
                                    flatten(car.position + 4000 * std::get<0>(tup)));
                renderer.DrawLine3D(Color::cyan(), car.position, car.position + 4000 * std::get<1>(tup));
                renderer.DrawLine3D(Color::cyan(), car.position, car.position + 4000 * std::get<2>(tup));
                vec2 intersect = getIntersection(car.position, std::get<0>(tup), vec2{ball_position}, direction);
                vec3 intersect3 = intersect;
                if (norm(intersect3) > 0)
                    renderer.DrawSphere(Color::red(), sphere{intersect3, 50});
                index++;
            }
        }
        vec3 shot = determineShotDirection(info.ball.position);
        renderer.DrawLine3D(Color::red(), ball_position, ball_position + 5000 * shot);

        vec3 left_post = -side(info.myCar) * vec3{893, 0, 0} + info.theirGoal;
        vec3 right_post = side(info.myCar) * vec3{893, 0, 0} + info.theirGoal;
        renderer.DrawLine3D(Color::purple(), right_post, right_post + vec3{0, 0, 3000});
        renderer.DrawLine3D(Color::cyan(), left_post, left_post + vec3{0, 0, 3000});
    }

}

std::tuple <vec3, vec3> Targeting::getConeDirection(vec3 original) {
    float distance_to_ball = distance(info.myCar.position, info.ball.position);
    float min = 0;
    float max = M_PI_4;
    float scale = std::min(1.f, distance_to_ball / 2500.f);
    float angle = scale * max + (1 - scale) * min;
    float d2_x = original[0] * cos(angle) - original[1] * sin(angle);
    float d2_y = original[0] * sin(angle) + original[1] * cos(angle);
    float d3_x = original[0] * cos(-angle) - original[1] * sin(-angle);
    float d3_y = original[0] * sin(-angle) + original[1] * cos(-angle);
    vec3 direction2 = {d2_x, d2_y, 0};
    vec3 direction3 = {d3_x, d3_y, 0};
    return std::make_tuple(direction2, direction3);
}

vec2 Targeting::getIntersection(vec2 start1, vec2 dir1, vec2 start2, vec2 dir2) {
    float a = ((start2[0] - start1[0]) * (-dir2[1]) - (start2[1] - start1[1]) * (-dir2[0])) / dir1[0] * (-dir2[1]) -
              (-dir2[0]) * dir1[1];
    float b = ((start2[1] - start1[1]) * (dir1[0]) - (start2[0] - start1[0]) * (dir1[1])) / dir1[0] * (-dir2[1]) -
              (-dir2[0]) * dir1[1];
    if (a > 0 and a < 8000 and b > 0 and b < 8000)
        return start1 + a * dir1;
    return vec2{0, 0};
}

vec2 Targeting::getClosestIntersect(vec2 start, vec2 shot, int index) {
    vec2 car_position = info.game.cars[opponent_indices[index]].position;
    std::tuple <vec3, vec3, vec3> tup = opponent_cones[index];
    vec2 result = {};
    float rdist = 99999999.f;
    vec2 temp = getIntersection(start, shot, car_position, std::get<0>(tup));
    if (norm(temp) > 0 and distance(car_position, temp) < rdist) {
        rdist = distance(car_position, temp);
        result = temp;

    }
    temp = getIntersection(start, shot, car_position, std::get<1>(tup));
    if (norm(temp) > 0 and distance(car_position, temp) < rdist) {
        rdist = distance(car_position, temp);
        result = temp;

    }
    temp = getIntersection(start, shot, car_position, std::get<2>(tup));
    if (norm(temp) > 0 and distance(car_position, temp) < rdist) {
        rdist = distance(car_position, temp);
        result = temp;

    }
    return result;
}

vec2 Targeting::determineShotDirection(vec3 ball_position) {
    vec3 direction = normalize(flatten(ball_position - info.myCar.position));
    std::tuple <vec3, vec3> ball_cone = getConeDirection(direction);
    vec3 left = std::get<0>(ball_cone);
    vec3 right = std::get<1>(ball_cone);
    std::vector <vec3> shots = generateShots(direction, left, right);
    vec2 result;
    float highest_score = -999;
    //score the shot vectors, higher = better
    for (vec3 shot : shots) {
        float score = 0;

        for (int i = 0; i < opponent_indices.size(); i++) {
            vec2 intersect = getClosestIntersect(ball_position, shot, i);
            if (norm(intersect) > 0) {
                // distance between the opponent and where the ball will cross them
                float distance_intersect_to_opponent = distance(flatten(info.game.cars[opponent_indices[i]].position), intersect);
                // distance between ball and the intersect
                float distance_ball_to_intersect = distance(ball_position, intersect);

                // shooting in the direction of the opponent is generally not so good
                score += clamp((distance_intersect_to_opponent) / distance_ball_to_intersect, 0.f, 2.f);
            }
            else score += 2.5;
        }
        //Shooting at the enemy net is good
        vec3 left_post = -side(info.myCar) * vec3{893, 0, 0} + info.theirGoal;
        vec3 right_post = side(info.myCar) * vec3{893, 0, 0} + info.theirGoal;
        if (postCorrection(ball_position, left_post, right_post)) {
            vec2 intersect_with_goal = getIntersection(vec2{-893, 0} + vec2{info.theirGoal}, vec2{1, 0}, ball_position,
                                                       shot);
            if (norm(intersect_with_goal) > 0) {
                if (abs(intersect_with_goal[0]) < 780) {
                    score += 3;
                }
            }
        }
        //Shooting at our own net is bad
        vec2 intersect_with_my_goal = getIntersection(vec2{-893, 0} + vec2{info.myGoal}, vec2{1, 0}, ball_position,
                                                      shot);
        if (norm(intersect_with_my_goal) > 0) {
            if (abs(intersect_with_my_goal[0]) < 780) score -= 2;
        }

        // encourages shots forward
        score -= side(info.myCar) * shot[1];

        // select shot with highest score
        if (score > highest_score) {
            result = shot;
            highest_score = score;
        }
    }
    return result;
}

std::vector <vec3> Targeting::generateShots(vec3 direction, vec3 left, vec3 right) {
    std::vector <vec3> shots = {};
    for (float i = 0; i < 1; i += 0.05) {
        shots.emplace_back(normalize(direction * i + left * (1 - i)));
        shots.emplace_back(normalize(direction * i + right * (1 - i)));
        shots.emplace_back(direction);
    }
    return shots;
}

bool Targeting::postCorrection(vec3 ball_position, vec3 left, vec3 right) {
    // GOSLING UTILS FUNCTION
    float ball_radius = 110.f;  // We purposely make this a bit larger so that our shots have a higher chance of success
    vec3 goal_line_perp = cross((right - left), vec3{0, 0, 1});
    vec3 left_adjusted = left + (cross(normalize(left - ball_position), vec3{0, 0, -1}) * ball_radius);
    vec3 right_adjusted = right + (cross(normalize(right - ball_position), vec3{0, 0, 1}) * ball_radius);
    vec3 left_corrected = left;
    if (dot(left_adjusted - left, goal_line_perp) <= 0.0) left_corrected = left_adjusted;
    vec3 right_corrected = right;
    if (dot(right_adjusted - right, goal_line_perp) <= 0.0) right_corrected = right_adjusted;
    float new_goal_width = distance(right_corrected, left_corrected);
    vec3 new_goal_line = normalize(right_corrected - left_corrected);
    vec3 new_goal_perp = cross(new_goal_line, vec3{0, 0, 1});
    vec3 goal_center = left_corrected + (new_goal_line * new_goal_width * 0.5);
    vec3 ball_to_goal = normalize(goal_center - ball_position);

    bool ball_fits = new_goal_width * abs(dot(new_goal_perp, ball_to_goal)) > ball_radius * 2;
    return ball_fits;
}
