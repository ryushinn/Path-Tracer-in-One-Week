#ifndef RAYTRACINGINONEWEEKEND_RTWEEKEND_H
#define RAYTRACINGINONEWEEKEND_RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
// usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degree_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // returns a real in [0, 1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // returns a real in [min, max)
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1));
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// common headers
#include "ray.h"
#include "vec3.h"
#include "color.h"

#endif //RAYTRACINGINONEWEEKEND_RTWEEKEND_H
