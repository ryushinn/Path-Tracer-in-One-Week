#ifndef RAYTRACINGINONEWEEKEND_AABB_H
#define RAYTRACINGINONEWEEKEND_AABB_H

#include "rtweekend.h"

class aabb {
public:
    point3 minimum, maximum;

    aabb() = default;

    aabb(const point3 &min, const point3 &max) : minimum(min), maximum(max) {}

    point3 min() const { return minimum; }

    point3 max() const { return maximum; }

    bool hit(const ray &r, double t_min, double t_max) const{
        for (int i = 0; i < 3; i++) {
            auto invD = 1.0f / r.direction()[i];
            auto t0 = (minimum[i] - r.origin()[i]) * invD;
            auto t1 = (maximum[i] - r.origin()[i]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t_max > t1 ? t1 : t_max;
            if (t_min >= t_max)
                return false;
        }
        return true;
    }
};

aabb surrounding_box(const aabb &box0, const aabb &box1) {
    point3 minimum(fmin(box0.min()[0], box1.min()[0]),
                   fmin(box0.min()[1], box1.min()[1]),
                   fmin(box0.min()[2], box1.min()[2]));
    point3 maximum(fmax(box0.max()[0], box1.max()[0]),
                   fmax(box0.max()[1], box1.max()[1]),
                   fmax(box0.max()[2], box1.max()[2]));

    return {minimum, maximum};
}

#endif //RAYTRACINGINONEWEEKEND_AABB_H
