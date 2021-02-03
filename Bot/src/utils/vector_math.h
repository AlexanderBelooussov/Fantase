#pragma once
#include "linear_algebra/math.h"
#include "linear_algebra/vec.h"
#include "linear_algebra/mat.h"
#include "simulation/car.h"


inline vec3 ground(const vec3 & pos)
{
    return vec3{pos[0], pos[1], 0};
}

inline float distance(const vec3 & obj1, const vec3 & obj2)
{
    return norm(obj1 - obj2);
}

inline float ground_distance(const vec3 & obj1, const vec3 & obj2)
{
    return norm(ground(obj1) - ground(obj2));
}

inline vec3 direction(const vec3 & source, const vec3 & target)
{
    return normalize(target - source);
}

inline vec3 ground_direction(const vec3 & source, const vec3 & target)
{
    return normalize(ground(target) - ground(source));
}

inline vec3 local(const Car & car, const vec3 & pos)
{
    return dot(pos - car.position, car.orientation);
}

inline vec3 local_velocity(const Car & car, const vec3 & vel)
{
    return dot(vel, car.orientation);
}

inline vec3 world(const Car & car, const vec3 & pos)
{
    return car.position + dot(car.orientation, pos);
}

inline float angle_to(const Car & car, const vec3 & target, bool backwards = false)
{
    return abs(angle_between(car.forward() * (backwards ? -1 : 1), direction(car.position, target)));
}

vec3 nearest_point(const vec3 & pos, const std::vector<vec3> & points);

vec3 furthest_point(const vec3 & pos, const std::vector<vec3> & points);

inline vec3 backsolve(Car& car, vec3 target, float time, float gravity){
    vec3 d = target - car.position;
    float dvx = ((d[0]/time) - car.velocity[0]) / time;
    float dvy = ((d[1]/time) - car.velocity[1]) / time;
    float dvz = (((d[2]/time) - car.velocity[2]) / time) + (-gravity * time);
    return vec3{dvx,dvy,dvz};
}