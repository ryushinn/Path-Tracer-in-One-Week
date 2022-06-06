#ifndef RAYTRACINGINONEWEEKEND_ISOTROPIC_H
#define RAYTRACINGINONEWEEKEND_ISOTROPIC_H

#include "rtweekend.h"
#include "material.h"
#include "texture.h"

class isotropic : public material {
public:
    shared_ptr<texture> albedo;

    explicit isotropic(const shared_ptr<texture>& a) : albedo(a) {}

    explicit isotropic(color c) : albedo(make_shared<solid_color>(c)) {}

    bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override {
        srec.pdf_ptr = make_shared<uniform_sphere_pdf>();
        return true;
    }

    color BxDF(const ray &r_in, const ray &r_out, const hit_record &rec) const override {
        return albedo->value(rec.u, rec.v, rec.p) / (4 * pi);
    }

};

#endif //RAYTRACINGINONEWEEKEND_ISOTROPIC_H
