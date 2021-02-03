//
// Created by Alex on 04/08/2020.
//

#include "Strategy.h"
#include "../maneuvers/Kickoff/FrontFlipKickoff.h"
#include "../maneuvers/Kickoff/SpeedFlipKickoff.h"
#include "../maneuvers/GetBoost.h"
#include "../maneuvers/MoveTo.h"
#include "../maneuvers/Shots/GroundShot.h"
#include "../maneuvers/Shots/DodgeShot.h"
#include "../maneuvers/Shots/AerialShot.h"
#include "../utils/misc.h"
#include "../utils/vector_math.h"

Strategy::Strategy(GameInfo& info): info(info){
    targeting = std::make_unique<Targeting>(info);
}

std::shared_ptr<Maneuver> Strategy::nextManeuver() {
    // Kickoff
    if (info.game.kickoff_pause){
        // Central
        if (abs(info.myCar.position[0]) < 1) return std::make_shared<FrontFlipKickoff>(info);
        // Diagonal or Off-center
        else {
            bool diagonal = true;
            int xside = 1;
            if (abs(info.myCar.position[0]) < 900) diagonal = false;
            if(info.myCar.position[0] < 0) xside = -1;
            return std::make_shared<SpeedFlipKickoff>(info, diagonal, xside);
        }
    }
    // recover in the air
    if (!info.myCar.on_ground) return std::make_shared<Recovery>(info);

    // find a shot

    std::shared_ptr<Shot> shot = std::make_shared<GroundShot>(info);
    std::shared_ptr<Shot> dodgeshot = std::make_shared<DodgeShot>(info);
    std::shared_ptr<Shot> aerial = std::make_shared<AerialShot>(info);
    shot->initialPrediction();
    dodgeshot->initialPrediction();
    aerial->initialPrediction();

    // find and set target
    targeting->generateOpponentCones();
    vec3 shot_target = shot->prediction.ball_position + 1000 * targeting->determineShotDirection(shot->prediction.ball_position);
    vec3 dodge_target = dodgeshot->prediction.ball_position + 1000 * targeting->determineShotDirection(dodgeshot->prediction.ball_position);
    vec3 aerial_target = aerial->prediction.ball_position + 1000 * targeting->determineShotDirection(aerial->prediction.ball_position);
    shot->target = shot_target;
    dodgeshot->target = dodge_target;
    aerial->target = aerial_target;
    if (shot->prediction.is_viable){
        if (!dodgeshot->prediction.is_viable or shot->prediction.time < dodgeshot->prediction.time + 0.5)
        return shot;
    }
    if (aerial->prediction.is_viable){
        //std::cout << aerial->prediction.ball_position << ", " << aerial->prediction.time - info.time << std::endl;
        if (aerial->prediction.time < dodgeshot->prediction.time)
            return aerial;
    }
    if (dodgeshot->prediction.is_viable) {
        return dodgeshot;
    }

    // no shot -> move to goal/get boost
    if (info.myCar.boost < 33) {
        std::shared_ptr<GetBoost> maneuver = std::make_shared<GetBoost>(info);
        maneuver->boost = findBoost();
        if (maneuver-> boost != -1)
            return maneuver;
    }
    std::shared_ptr<MoveTo> maneuver = std::make_shared<MoveTo>(info);
    maneuver->target = info.myGoal;
    return maneuver;
}

int Strategy::findBoost(bool only_behind_ball, std::unique_ptr<vec3> close_point) {
    std::vector<int> boost_pads = {};
    for (int i = 0; i < info.game.pads.size(); i++) {
        Pad &pad = info.game.pads.at(i);
        if (pad.is_full_boost and pad.is_active) {
            if (not only_behind_ball or
                side(info.myCar) * pad.position[1]
                > clamp<float>(side(info.myCar) * info.ball.position[1] + 90, -6000, 4090)) {
                boost_pads.push_back(i);
            }
        }
    }
    int boost = -1;
    for (int i : boost_pads) {
        Pad &pad = info.game.pads.at(i);
        if (boost == -1) {
            boost = i;
        } else if (close_point) {
            if (distance(*close_point, pad.position) < distance(*close_point, info.game.pads.at(boost).position)) {
                boost = i;
            }
        } else {
            if (distance(info.myCar.position, pad.position) <
                distance(info.myCar.position, info.game.pads.at(boost).position)) {
                boost = i;
            }
        }
    }
    return boost;
}