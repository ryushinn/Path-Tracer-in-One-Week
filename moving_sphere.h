#ifndef RAYTRACINGINONEWEEKEND_MOVING_SPHERE_H
#define RAYTRACINGINONEWEEKEND_MOVING_SPHERE_H

#include "hittable.h"

class moving_sphere : public hittable {
public:
    point3 center0, center1;
    double radius;
    double time0, time1;
    shared_ptr<material> mat_ptr;

    moving_sphere(const point3 &center0, const point3 &center1, double time0, double time1, double radius,
                  shared_ptr<material> &m) : center0(center0), center1(center1), time0(time0), time1(time1),
                                             radius(radius), mat_ptr(m) {};

    point3 center(double t) const;

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override;
};

bool moving_sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto oc = r.origin() - center(r.time());
    auto A = dot(r.direction(), r.direction());
    auto half_B = dot(r.direction(), oc);
    auto C = oc.length_squared() - radius * radius;

    auto determinant = half_B * half_B - A * C;
    if (determinant < 0) {
        return false;
    } else {
        auto t = (-half_B - sqrt(determinant)) / A;
        if (t > t_max)
            return false;
        if (t < t_min) {
            t = (-half_B + sqrt(determinant)) / A;
            if (t < t_min || t > t_max)
                return false;
        }
        rec.p = r.at(t);
        auto outward_normal = (rec.p - center(r.time())) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.t = t;
        rec.mat_ptr = mat_ptr;
        return true;
    }
}

point3 moving_sphere::center(double t) const {
    return center0 + (t - time0) / (time1 - time0) * (center1 - center0);
}

bool moving_sphere::bounding_box(double t0, double t1, aabb &output_box) const {
    aabb box0(center(t0) - vec3(radius), center(t0) + vec3(radius));
    aabb box1(center(t1) - vec3(radius), center(t1) + vec3(radius));
    output_box = surrounding_box(box0, box1);
    return true;
}

#endif //RAYTRACINGINONEWEEKEND_MOVING_SPHERE_H
