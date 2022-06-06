#ifndef RAYTRACINGINONEWEEKEND_LAMBERTIAN_H
#define RAYTRACINGINONEWEEKEND_LAMBERTIAN_H

#include <utility>

#include "material.h"
#include "texture.h"
#include "onb.h"

class lambertian: public material {
public:
    shared_ptr<texture> albedo;

    lambertian() = default;

    explicit lambertian(const color& albedo) : albedo(make_shared<solid_color>(albedo)) {}

    explicit lambertian(shared_ptr<texture> t) : albedo(std::move(t)) {}

    bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override {
        // Some notes from the first book:
        // See https://github.com/vchizhov/Derivations
        // for math proofs on pdfs of projected offset ball and sphere

        // pdf ~ cos(theta)
        // vec3 scatter_direction = rec.normal + random_on_unit_sphere();

        // pdf ~ cos(theta)^3
        // vec3 scatter_direction = rec.normal + random_in_unit_sphere();

        srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
        return true;
    }

    color BxDF(const ray &r_in, const ray &r_out, const hit_record &rec) const override {
        return albedo->value(rec.u, rec.v, rec.p) * dot(rec.normal, normalize(r_out.direction())) / pi;
    }

};

#endif //RAYTRACINGINONEWEEKEND_LAMBERTIAN_H
