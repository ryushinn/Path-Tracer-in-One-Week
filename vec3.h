#ifndef RAYTRACINGINONEWEEKEND_VEC3_H
#define RAYTRACINGINONEWEEKEND_VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
public:
    double e[3];

    vec3() : e{0, 0, 0} {};

    vec3(double all) : e{all, all, all} {};

    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {};

    double x() const { return e[0]; }

    double y() const { return e[1]; }

    double z() const { return e[2]; }

    vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }

    double operator[](int i) const { return e[i]; }

    double &operator[](int i) { return e[i]; }

    vec3 &operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3 &operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(double t) {
        return *this *= 1 / t;
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    double length() const {
        return sqrt(this->length_squared());
    }

    inline static vec3 random() {
        return {random_double(), random_double(), random_double()};
    }

    inline static vec3 random(double min, double max) {
        return {random_double(min, max), random_double(min, max), random_double(min, max)};
    }

    bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};


using point3 = vec3;
using color = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v[0] << " " << v[1] << " " << v[2];
}

inline vec3 operator+(const vec3 &v0, const vec3 &v1) {
    return {v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2]};
}

inline vec3 operator-(const vec3 &v0, const vec3 &v1) {
    return {v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]};
}

inline vec3 operator*(const vec3 &v0, const vec3 &v1) {
    return {v0[0] * v1[0], v0[1] * v1[1], v0[2] * v1[2]};
}

inline vec3 operator*(double t, const vec3 &v) {
    return {t * v[0], t * v[1], t * v[2]};
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3 &v, double t) {
    return (1 / t) * v;
}

inline vec3 abs(const vec3 &v) {
    return {abs(v[0]), abs(v[1]), abs(v[2])};
}

inline bool operator==(const vec3 &lhs, const vec3 &rhs) {
    return (lhs-rhs).near_zero();
}

inline bool operator!=(const vec3 &lhs, const vec3 &rhs) {
    return !(lhs == rhs);
}

inline double dot(const vec3 &v0, const vec3 &v1) {
    return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

inline vec3 cross(const vec3 &v0, const vec3 &v1) {
    return {v0[1] * v1[2] - v0[2] * v1[1],
            -(v0[0] * v1[2] - v0[2] * v1[0]),
            v0[0] * v1[1] - v0[1] * v1[0]};
}

inline bool parallel(const vec3 &v0, const vec3 &v1) {
    return cross(v0, v1).near_zero();
}

inline vec3 axis_rot(int axis, const vec3 &v, double radians) {
    auto _sin = sin(radians);
    auto _cos = cos(radians);
    axis %= 3;
    auto index0 = (axis + 1) % 3;
    auto index1 = (index0 + 1) % 3;

    vec3 rotated_v(v);
    rotated_v[index0] = _cos * v[index0] - _sin * v[index1];
    rotated_v[index1] = _sin * v[index0] + _cos * v[index1];
    return rotated_v;
}

inline vec3 normalize(const vec3 &v) {
    return v / v.length();
}

vec3 random_in_unit_sphere() {
    vec3 res;
    do {
        res = vec3::random(-1, 1);
    } while (res.length() >= 1);
    return res;
}

vec3 random_on_unit_sphere() {
    return normalize(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3 &n) {
    vec3 res = random_in_unit_sphere();
    if (dot(res, n) > 0.0)
        return res;
    else
        return -res;
}

inline vec3 random_cosine_direction() {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1-r2);

    auto phi = 2*pi*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    return {x, y, z};
}

vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3 &v, const vec3 &n, double etai_over_etat) {
    vec3 v_unit = normalize(v);
    vec3 n_unit = normalize(n);
    vec3 r_perpendicular = etai_over_etat * (v_unit + dot(-v_unit, n_unit) * n_unit);
    vec3 r_parallel = -sqrt(fabs(1 - r_perpendicular.length_squared())) * n;
    return r_perpendicular + r_parallel;
}

#endif //RAYTRACINGINONEWEEKEND_VEC3_H
