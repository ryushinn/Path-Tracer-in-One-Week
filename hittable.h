#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_H

#include <utility>

#include "vec3.h"
#include "ray.h"
#include "aabb.h"


class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u, v;
    bool front_face;

    inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const = 0;

    virtual double pdf_value(const point3 &o, const vec3 &v) const{
        return 0.0;
    }

    virtual vec3 random(const vec3 &o) const {
        return {1.0, 0.0, 0.0};
    }
};

class translate : public hittable {
public:
    shared_ptr<hittable> ptr;
    vec3 offset;

    translate(const shared_ptr<hittable> &p, const vec3 &displacement) : ptr(p), offset(displacement) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override;
};

bool translate::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    ray translated_ray(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(translated_ray, t_min, t_max, rec))
        return false;
    rec.p += offset;
    return true;
}

bool translate::bounding_box(double time0, double time1, aabb &output_box) const {
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

    output_box = aabb(output_box.min() + offset, output_box.max() + offset);
    return true;
}

class axis_rotate : public hittable {
public:
    shared_ptr<hittable> ptr;
    double theta;
    int axis;

    axis_rotate(shared_ptr<hittable> p, int _axis, double angle) : ptr(std::move(p)), axis(_axis),
                                                                   theta(degree_to_radians(angle)) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override;
};

bool axis_rotate::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    ray rotated_ray(axis_rot(axis, r.origin(), -theta),
                    axis_rot(axis, r.direction(), -theta), r.time());

    if (!ptr->hit(rotated_ray, t_min, t_max, rec))
        return false;

    rec.p = axis_rot(axis, rec.p, theta);
    rec.normal = axis_rot(axis, rec.normal, theta);
    return true;
}

bool axis_rotate::bounding_box(double time0, double time1, aabb &output_box) const {
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i * output_box.min().x() + (1 - i) * output_box.max().x();
                auto y = j * output_box.min().y() + (1 - j) * output_box.max().y();
                auto z = k * output_box.min().z() + (1 - k) * output_box.max().z();

                point3 tester = axis_rot(axis, point3(x, y, z), theta);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }
    output_box = aabb(min, max);
    return true;
}

class flip_face : public hittable {
public:
    flip_face(shared_ptr<hittable> p) : ptr(p) {}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override {

        if (!ptr->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face = !rec.front_face;
        return true;
    }

    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override {
        return ptr->bounding_box(time0, time1, output_box);
    }

public:
    shared_ptr<hittable> ptr;
};

#endif //RAYTRACINGINONEWEEKEND_HITTABLE_H
