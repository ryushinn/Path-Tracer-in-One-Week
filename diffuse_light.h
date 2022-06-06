#ifndef RAYTRACINGINONEWEEKEND_DIFFUSE_LIGHT_H
#define RAYTRACINGINONEWEEKEND_DIFFUSE_LIGHT_H
#include <utility>

#include "rtweekend.h"
#include "material.h"
#include "texture.h"
class diffuse_light : public material {
public:
    shared_ptr<texture> emit;

    diffuse_light() = default;
    explicit diffuse_light(shared_ptr<texture> t) : emit(std::move(t)) {}

    bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const override {
        return false;
    };

    color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override{
        if (rec.front_face)
            return emit->value(u, v, p);

        return {0.0};
    }

};


#endif //RAYTRACINGINONEWEEKEND_DIFFUSE_LIGHT_H
