//
// Created by Alex on 02/11/2020.
//

#ifndef CPPBOT_TARGETING_H
#define CPPBOT_TARGETING_H


#include <rlbot/renderer.h>
#include "../utils/game_info.h"

class Targeting {
    GameInfo info;
    std::vector<int> opponent_indices;
    std::vector<std::tuple<vec3, vec3, vec3>> opponent_cones;

    std::tuple<vec3, vec3> getConeDirection(vec3 original);
    vec2 getIntersection(vec2 start1, vec2 dir1, vec2 start2, vec2 dir2);
    vec2 getClosestIntersect(vec2 start, vec2 shot, int index);
    std::vector<vec3>generateShots(vec3 direction, vec3 left, vec3 right);
    bool postCorrection(vec3 ball_position, vec3 left, vec3 right);
public:
    Targeting(const GameInfo &info);

    void generateOpponentCones();

    vec2 determineShotDirection(vec3 ball_position);

    virtual void render(Renderer & renderer);
};


#endif //CPPBOT_TARGETING_H
