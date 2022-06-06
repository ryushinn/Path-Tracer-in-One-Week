#ifndef RAYTRACINGINONEWEEKEND_CAMERA_H
#define RAYTRACINGINONEWEEKEND_CAMERA_H

#include "rtweekend.h"

class camera {
public:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;

    camera() : camera(
            {13, 2, 3},
            {0, 0, 0},
            {0, 1, 0},
            40.0,
            1.0,
            0.0,
            10.0,
            0.0, 1.0
            ) {}

    camera(point3 lookfrom,
           point3 lookat,
           vec3 vup,
           double vfov, // vertical fov in degrees
           double aspect_ratio,
           double aperture,
           double focus_dist,
           double _time0, double _time1) {
        time0 = _time0;
        time1 = _time1;
        w = -normalize(lookat - lookfrom);
        u = cross(normalize(vup), w);
        v = cross(w, u);

        auto theta = degree_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = viewport_height * aspect_ratio;

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - focus_dist * w - horizontal / 2 - vertical / 2;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const {
        double u_offset = lens_radius * random_double(-1, 1);
        double v_offset = lens_radius * random_double(-1, 1);
        vec3 origin_offset = origin + u * u_offset + v * v_offset;

        return ray(origin_offset, lower_left_corner + s * horizontal + t * vertical - origin_offset,
                   random_double(time0, time1));
    }
};

#endif //RAYTRACINGINONEWEEKEND_CAMERA_H
