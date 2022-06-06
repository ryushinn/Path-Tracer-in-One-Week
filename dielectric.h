#ifndef RAYTRACINGINONEWEEKEND_DIELECTRIC_H
#define RAYTRACINGINONEWEEKEND_DIELECTRIC_H

#include "material.h"

class dielectric : public material {
private:
    static vec3 compute_scattered_dir(const vec3 incident, const vec3 normal, double refraction_ratio) {
        vec3 unit_direction = normalize(incident);
        double cos_eta = dot(normal, -unit_direction);
        double sin_eta = sqrt(1 - cos_eta * cos_eta);
        vec3 scattered_dir;
        if (refraction_ratio * sin_eta > 1 || reflectance(cos_eta, refraction_ratio) > random_double()) {
            scattered_dir = reflect(unit_direction, normal);
        } else {
            scattered_dir = refract(unit_direction, normal, refraction_ratio);
        }
        return scattered_dir;
    }
public:
    double ir{}; // index of refraction

    dielectric() = default;

    explicit dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override {
        double refraction_ratio = rec.front_face ? 1.0 / ir : ir;
        vec3 scattered_dir = compute_scattered_dir(r_in.direction(), rec.normal, refraction_ratio);

        srec.pdf_ptr = make_shared<dirac_delta_pdf>(scattered_dir);
        srec.is_delta = true;
        return true;
    }

    color BxDF(const ray &r_in, const ray &r_out, const hit_record &rec) const override {
        double refraction_ratio = rec.front_face ? 1.0 / ir : ir;
        vec3 scattered_dir = compute_scattered_dir(r_in.direction(), rec.normal, refraction_ratio);

        return parallel(scattered_dir, r_out.direction()) ? 1.0 / DELTA_VALUE : 0;
    }

private:
    static double reflectance(double cosine, double ref_idx) {
        auto f0 = (1 - ref_idx) / (1 + ref_idx);
        f0 = f0 * f0;
        return f0 + (1 - f0) * pow((1 - cosine), 5);
    }
};

#endif //RAYTRACINGINONEWEEKEND_DIELECTRIC_H
