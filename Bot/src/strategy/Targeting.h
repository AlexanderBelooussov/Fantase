//
// Created by Alex on 02/11/2020.
//

#ifndef CPPBOT_TARGETING_H
#define CPPBOT_TARGETING_H


#include <rlbot/renderer.h>
#include "../utils/game_info.h"

/**
 * A class used for determining what direction to shoot at
 * Looks at opponents position/direction and whether it can shoot at the goal
 */
class Targeting {
    GameInfo info;
    std::vector<int> opponent_indices;
    std::vector <std::tuple<vec3, vec3, vec3>> opponent_cones;

    /**
     * Calculates the left and right edge of the cone
     * @param original: The forward direction
     * @return Left and right vectors of the cone
     * @todo make it so only the ball cone changes size
     */
    std::tuple <vec3, vec3> getConeDirection(vec3 original);

    /**
     * 2D intersection of 2 lines
     * @param start1: Starting point of the first line
     * @param dir1: Direction of the first line
     * @param start2: Starting point of the second line
     * @param dir2: Direction of the second line
     * @return 2D vector of the intersection OR {0, 0} if there is none
     */
    vec2 getIntersection(vec2 start1, vec2 dir1, vec2 start2, vec2 dir2);

    /**
     * Gets the point at which our shot intersects the opponent's cone
     * @param start: The position the ball starts at
     * @param shot: The direction of the shot
     * @param index: Index of the opponent
     * @return 2 Dimensional vector
     */
    vec2 getClosestIntersect(vec2 start, vec2 shot, int index);

    /**
     * Generates a number of directional vectors in a certain cone
     * @param direction: the center of the shot cone
     * @param left: left edge of the shot cone
     * @param right: right edge of the shot cone
     * @return A std::vector of shot vectors
     */
    std::vector <vec3> generateShots(vec3 direction, vec3 left, vec3 right);

    /**
     * Gosling utils function to see whether a shot fits between the posts
     * @param ball_position
     * @param left: Left post
     * @param right: Right post
     * @return True if the ball fits
     */
    bool postCorrection(vec3 ball_position, vec3 left, vec3 right);

public:
    Targeting(const GameInfo &info);

    /**
     * Generates vision-like cones for each opponent
     * Each cone is actually just 3 lines, and is used to see if a shot crosses an opponents path
     * Should be done before calculating the direction of the shot
     */
    void generateOpponentCones();

    /**
     * Calculates the best direction to shoot the ball to
     * @param ball_position: Specifies the location off the ball, usually the predicted intercept point
     * @return 2 Dimensional vector of the shot direction
     */
    vec2 determineShotDirection(vec3 ball_position);

    virtual void render(Renderer &renderer);
};


#endif //CPPBOT_TARGETING_H
