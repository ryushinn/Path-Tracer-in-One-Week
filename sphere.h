#ifndef RAYTRACINGINONEWEEKEND_SPHERE_H
#define RAYTRACINGINONEWEEKEND_SPHERE_H

#include <utility>

#include "hittable.h"
#include "onb.h"

class sphere : public hittable {
public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;

    sphere(const point3 &center, double radius, shared_ptr<material> m) : center(center), radius(radius),
                                                                          mat_ptr(std::move(m)) {};

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override;

    double pdf_value(const point3 &o, const vec3 &v) const override;

    vec3 random(const vec3 &o) const override;

private:
    static void get_sphere_uv(const point3 &p, double &u, double &v) {
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    auto oc = r.origin() - center;
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
        auto outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.t = t;
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat_ptr = mat_ptr;
        return true;
    }
}

bool sphere::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = aabb(
            center - vec3(radius),
            center + vec3(radius)
    );
    return true;
}

double sphere::pdf_value(const point3 &o, const vec3 &v) const {
    hit_record rec;
    if (!this->hit(ray(o, v), 0.001, infinity, rec))
        return 0;

    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).length_squared());
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

inline vec3 random_to_sphere(double radius, double distance_squared) {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

    auto phi = 2 * pi * r1;
    auto x = cos(phi) * sqrt(1 - z * z);
    auto y = sin(phi) * sqrt(1 - z * z);

    return {x, y, z};
}

vec3 sphere::random(const point3 &o) const {
    vec3 direction = center - o;
    auto distance_squared = direction.length_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}

#endif //RAYTRACINGINONEWEEKEND_SPHERE_H
