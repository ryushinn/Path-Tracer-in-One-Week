#ifndef RAYTRACINGINONEWEEKEND_AARECT_H
#define RAYTRACINGINONEWEEKEND_AARECT_H

#include <utility>

#include "rtweekend.h"
#include "hittable.h"

class xy_rect : public hittable {
public:
    double x0, x1, y0, y1, k;
    shared_ptr<material> mp;

    xy_rect() = default;

    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, const shared_ptr<material> &mat) : x0(_x0),
                                                                                                          x1(_x1),
                                                                                                          y0(_y0),
                                                                                                          y1(_y1),
                                                                                                          k(_k),
                                                                                                          mp(mat) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override {
        output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
        return true;
    }


};

bool xy_rect::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto t = (k - r.origin().z()) / r.direction().z();

    if (t < t_min || t > t_max)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;

    rec.mat_ptr = mp;
    vec3 outward_normal(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.p = point3(x, y, k);
    rec.t = t;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    return true;
}

class xz_rect : public hittable {
public:
    xz_rect() = default;

    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, const shared_ptr<material> &mat) : x0(_x0),
                                                                                                          x1(_x1),
                                                                                                          z0(_z0),
                                                                                                          z1(_z1),
                                                                                                          k(_k),
                                                                                                          mp(mat) {};

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override {
        output_box = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
        return true;
    }

    double pdf_value(const point3 &o, const vec3 &v) const override;

    vec3 random(const vec3 &o) const override;

public:
    shared_ptr<material> mp;
    double x0, x1, z0, z1, k;
};

bool xz_rect::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto t = (k - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

double xz_rect::pdf_value(const point3 &o, const vec3 &v) const {
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto area = (x1-x0)*(z1-z0);
    auto distance_squared = rec.t * rec.t * v.length_squared();
    auto cosine = fabs(dot(v, rec.normal) / v.length());

    return distance_squared / (cosine * area);
}

vec3 xz_rect::random(const vec3 &o) const {
    auto random_point = point3(random_double(x0,x1), k, random_double(z0,z1));
    return random_point - o;
}

class yz_rect : public hittable {
public:
    yz_rect() = default;

    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, const shared_ptr<material> &mat) : y0(_y0),
                                                                                                          y1(_y1),
                                                                                                          z0(_z0),
                                                                                                          z1(_z1),
                                                                                                          k(_k),
                                                                                                          mp(mat) {};

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override {
        output_box = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
        return true;
    }

public:
    shared_ptr<material> mp;
    double y0, y1, z0, z1, k;
};

bool yz_rect::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto t = (k - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;

    auto y = r.origin().y() + t * r.direction().y();
    auto z = r.origin().z() + t * r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

#endif //RAYTRACINGINONEWEEKEND_AARECT_H
