#ifndef RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H
#define RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H

#include <memory>
#include <utility>
#include <vector>
#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() = default;

    explicit hittable_list(shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override;

    double pdf_value(const point3 &o, const vec3 &v) const override;

    vec3 random(const vec3 &o) const override;

    int size() const {
        return static_cast<int>(objects.size());
    }
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    bool hit_anything = false;

    for (const auto &it : objects) {
        bool is_hit = it->hit(r, t_min, t_max, rec);
        if (is_hit) {
            hit_anything = true;
            t_max = rec.t;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb &output_box) const {
    if (objects.empty())
        return false;

    aabb temp_box;
    bool first_box = true;
    for (const auto& object: objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }
    return true;
}

double hittable_list::pdf_value(const point3& o, const vec3& v) const {
    auto weight = 1.0/objects.size();
    auto sum = 0.0;

    for (const auto& object : objects)
        sum += weight * object->pdf_value(o, v);

    return sum;
}

vec3 hittable_list::random(const vec3& o) const {
    auto int_size = static_cast<int>(objects.size());
    return objects[random_int(0, int_size-1)]->random(o);
}

#endif //RAYTRACINGINONEWEEKEND_HITTABLE_LIST_H
