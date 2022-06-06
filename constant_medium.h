#ifndef RAYTRACINGINONEWEEKEND_CONSTANT_MEDIUM_H
#define RAYTRACINGINONEWEEKEND_CONSTANT_MEDIUM_H

#include "rtweekend.h"
#include "hittable.h"
#include "isotropic.h"

class constant_medium : public hittable {
public:
    double density;
    shared_ptr<material> phase_function;
    shared_ptr<hittable> boundary;

    constant_medium(const shared_ptr<hittable> &b, double d, const shared_ptr<texture> &a)
            : boundary(b), density(d), phase_function(make_shared<isotropic>(a)) {}

    constant_medium(const shared_ptr<hittable> &b, double d, color c)
            : boundary(b), density(d),
              phase_function(make_shared<isotropic>(make_shared<solid_color>(c))) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override {
        return boundary->bounding_box(time0, time1, output_box);
    }
};

bool constant_medium::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    const bool enable_debug = false;
    const bool debugging = enable_debug && random_double() < 0.00001;

    hit_record rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t + 0.0001, infinity, rec2))
        return false;

    if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << "\n";

    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;

    // todo: why? how to interpret this probability?
    const auto hit_distance = -1 * log(random_double()) / density;

    if (hit_distance > distance_inside_boundary)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << "\n"
                  << "rec.t = " << rec.t << "\n"
                  << "rec.p = " << rec.p << "\n";
    }

    rec.normal = vec3(1, 0, 0);
    rec.front_face = true;
    rec.mat_ptr = phase_function;

    return true;
}

#endif //RAYTRACINGINONEWEEKEND_CONSTANT_MEDIUM_H
