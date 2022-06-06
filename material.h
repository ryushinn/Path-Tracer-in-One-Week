#ifndef RAYTRACINGINONEWEEKEND_MATERIAL_H
#define RAYTRACINGINONEWEEKEND_MATERIAL_H

#include "rtweekend.h"
#include "pdf.h"

struct hit_record;

struct scatter_record {
    bool is_delta = false;
    shared_ptr<pdf> pdf_ptr;
};

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, scatter_record &srec) const = 0;

    virtual color BxDF(const ray &r_in, const ray &r_out, const hit_record &rec) const {
        return {0.0};
    }

    virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3 &p) const {
        return {0.0};
    }
};

#endif //RAYTRACINGINONEWEEKEND_MATERIAL_H
