//
// Created by Alex on 03/08/2020.
//

#ifndef CPPBOT_SPEEDFLIPKICKOFF_H
#define CPPBOT_SPEEDFLIPKICKOFF_H


#include <mechanics/drive.h>
#include <mechanics/dodge.h>
#include "../Maneuver.h"

/**
 * Kickoff using a speed flip
 * Used for any non-center spawn
 * @todo make diagonal spawns hit ball more centrally
 */
class SpeedFlipKickoff : public Maneuver {
    int fase = 1;
    Drive drive;
    Dodge dodge;
    Reorient reorient;
    float sf_start = -1;
    int dir = 1;
    float x, y, y_close;
    bool dodging = false;
    bool ready = false;
    float resume_time = 0;

    void setup();

    /**
     * If the enemy is not going for a kickoff, or is very slow, it's better to not dodge into the ball
     */
    bool shouldDodge();

    /**
     * executes a speed flip
     * @param turn: whether we want to turn in the opposite direction before flipping (used for diagonal spawns)
     * @param direction: direction of the flip, 1 = right, -1 = left (iirc)
     */
    bool speedFlip(int direction, bool turn = true);

    /**
     * Final step of the kickoff, dodging into the ball
     */
    void dodgeIntoBall();
public:
    /**
     * Whether the bot starts on the diagonal/closer spawn
     */
    bool diagonal = true;
    /**
     * Whether the bot starts on the positive x side or negative x side
     */
    int xside = 1;

    SpeedFlipKickoff(GameInfo &info, bool diagonal, int xside) : Maneuver(info), drive(info.myCar), dodge(info.myCar), reorient(info.myCar), diagonal(diagonal), xside(xside) {
        drive.speed = 2300;
        interuptable = false;

    };

    void step() override;

    void render(Renderer &renderer) override;
};


#endif //CPPBOT_SPEEDFLIPKICKOFF_H
