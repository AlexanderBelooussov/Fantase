#include "rlbot/bot.h"
#include "mechanics/drive.h"
#include "simulation/game.h"
#include "simulation/car.h"
#include "simulation/field.h"
#include "rlbot/renderer.h"
#include "agent.h"
#include <memory>
#include <ctime>

#include "maneuvers/MoveTo.h"
#include "maneuvers/Kickoff/SpeedFlipKickoff.h"
#include "maneuvers/Kickoff/FrontFlipKickoff.h"

Albux::Fantase(int index, int team, std::string name, Game &game)
        : Bot::Bot(index, team, name), renderer(index), info(game, index, team) {
    Field::initialize_soccar();
    maneuver = nullptr;
    srand(time(NULL)); // random seed for state setting
    strategy = std::make_unique<Strategy>(info);
}

Input Fantase::GetOutput(Game game) {
    info.update();


    //reset maneuver on touch
    if (game.latest_touch_time != 0) {
        if (this->latest_touch_time != game.latest_touch_time
            and game.latest_touch_player != game.cars[index].name
            and maneuver->interuptable) {
            maneuver = nullptr;
            latest_touch_time = game.latest_touch_time;
            hasCancelledManeuverDueToKickoff = false;
        }
    }

    //reset on kickoff
    if (game.kickoff_pause and not hasCancelledManeuverDueToKickoff) {
        maneuver = nullptr;
        hasCancelledManeuverDueToKickoff = true;
    }

    //finished maneuver
    if (maneuver and maneuver->finished) {
        maneuver = nullptr;
        hasCancelledManeuverDueToKickoff = false;
    }

    //new maneuver
    if (maneuver == nullptr) {
        maneuver = strategy->nextManeuver();
    }

    //execute maneuver
    maneuver->step();

    // rendering
    renderer.Start();
    maneuver->render(renderer);
    strategy->targeting->generateOpponentCones();
    strategy->targeting->render(renderer);
    renderer.DrawString3D({255, 255, 255, 255}, typeid(*maneuver).name(), info.myCar.position, 1, 1);
    renderer.Finish();


    //return controls
    return maneuver->controls;

}

