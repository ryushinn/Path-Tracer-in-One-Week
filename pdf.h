#ifndef RAYTRACINGINONEWEEKEND_PDF_H
#define RAYTRACINGINONEWEEKEND_PDF_H

#include <utility>

#include "onb.h"
#include "vec3.h"
#include "rtweekend.h"

/*
 * To write math code completely and clearly, this macro serves as a symbolic placeholder of the value of Dirac delta function.
 * Since delta value will always be cancelled out in computations, it can be an arbitrary value and 1 is chosen here.
 */
#define DELTA_VALUE 1

class pdf {
public:
    virtual ~pdf() = default;

    virtual double value(const vec3 &direction) const = 0;

    virtual vec3 generate() const = 0;
};


class cosine_pdf : public pdf {
public:
    explicit cosine_pdf(const vec3 &w) { uvw.build_from_w(w); }

    double value(const vec3 &direction) const override {
        auto cosine = dot(normalize(direction), uvw.w());
        return fmax(1e-8, cosine / pi);
    }

    vec3 generate() const override {
        return uvw.local(random_cosine_direction());
    }

public:
    onb uvw{};
};

class hittable_pdf : public pdf {
public:
    hittable_pdf(shared_ptr<hittable> p, const point3 &origin) : ptr(std::move(p)), o(origin) {}

    double value(const vec3 &direction) const override {
        return ptr->pdf_value(o, direction);
    }

    vec3 generate() const override {
        return ptr->random(o);
    }

public:
    point3 o;
    shared_ptr<hittable> ptr;
};

class mixture_pdf : public pdf {
public:
    mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1) {
        p[0] = std::move(p0);
        p[1] = std::move(p1);
    }

    double value(const vec3 &direction) const override {
        return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
    }

    vec3 generate() const override {
        if (random_double() < 0.5)
            return p[0]->generate();
        else
            return p[1]->generate();
    }

public:
    shared_ptr<pdf> p[2];
};

class uniform_sphere_pdf : public pdf {
public:
    double value(const vec3 &direction) const override {
        return 1 / (4 * pi);
    }

    vec3 generate() const override {
        return random_on_unit_sphere();
    }
};

class dirac_delta_pdf : public pdf {
public:
    explicit dirac_delta_pdf(vec3 dir) : impulse(normalize(dir)) {}

    double value(const vec3 &direction) const override {
        return parallel(impulse, direction) ? DELTA_VALUE : 0;
    }

    vec3 generate() const override {
        return impulse;
    }

public:
    vec3 impulse;
};


#endif //RAYTRACINGINONEWEEKEND_PDF_H
