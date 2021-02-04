#pragma once
#include "rlbot/bot.h"
#include "simulation/game.h"
#include "simulation/car.h"
#include "rlbot/renderer.h"
#include "utils/game_info.h"
#include "maneuvers/Maneuver.h"
#include "strategy/Strategy.h"
#include <memory>

/**
 * Agent for the bot itself
 */
class Fantase : public Bot
{
    GameInfo info;
    std::shared_ptr<Maneuver> maneuver;
    Renderer renderer;

    bool hasCancelledManeuverDueToKickoff = false;

    float latest_touch_time = 0;
    int tick_counter = 0;



    std::unique_ptr<Strategy> strategy;



public:
    Fantase(int index, int team, std::string name, Game & game);

    Input GetOutput(Game game);

    //std::shared_ptr<Maneuver> chooseManeuver();
};