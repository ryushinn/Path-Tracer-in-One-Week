#ifndef RAYTRACINGINONEWEEKEND_RAY_H
#define RAYTRACINGINONEWEEKEND_RAY_H

#include "vec3.h"

class ray {
public:
    point3 orig;
    vec3 dir;
    double tm;

    ray() : orig(0, 0, 0), dir(1, 1, 1) {};

    ray(const point3 &origin, const vec3 &direction) : orig(origin), dir(direction), tm(0) {};

    ray(const point3 &origin, const vec3 &direction, double time) : orig(origin), dir(direction), tm(time) {};

    point3 at(double t) const {
        return orig + t * dir;
    }

    point3 origin() const { return orig; }

    vec3 direction() const { return dir; }

    double time() const { return tm; }
};

#endif //RAYTRACINGINONEWEEKEND_RAY_H
