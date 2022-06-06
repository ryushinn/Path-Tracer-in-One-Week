#ifndef RAYTRACINGINONEWEEKEND_METAL_H
#define RAYTRACINGINONEWEEKEND_METAL_H

#include "material.h"

class metal : public material {

public:
    color albedo;
    double fuzz;

    metal() {}

    metal(color a, double f) : albedo(a), fuzz(f < 1 ? f >= 0 ? f : 0 : 1) {}

    bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override {
        vec3 ref = reflect(normalize(r_in.direction()), rec.normal);
        vec3 impulse = ref + fuzz * random_in_unit_sphere();
        srec.pdf_ptr = make_shared<dirac_delta_pdf>(impulse);
        srec.is_delta = true;
        return (dot(impulse, rec.normal) > 0);
    }

    color BxDF(const ray &r_in, const ray &r_out, const hit_record &rec) const override {
        // Theoretically, it is needed to check the r_in and r_out, but we omit here by assuming that
        // mixture PDF will not be applied onto Dirac Delta PDF
        return albedo / DELTA_VALUE;
    }
};

#endif //RAYTRACINGINONEWEEKEND_METAL_H
