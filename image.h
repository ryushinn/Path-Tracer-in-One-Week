#ifndef RAYTRACINGINONEWEEKEND_IMAGE_H
#define RAYTRACINGINONEWEEKEND_IMAGE_H

#include "rtweekend.h"

#include "hittable_list.h"
#include "moving_sphere.h"
#include "sphere.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "aarect.h"
#include "bvh_node.h"
#include "box.h"
#include "constant_medium.h"
#include "image.h"
#include <iostream>

struct image {
    int width;
    int height;
    bvh_node world;
    shared_ptr<hittable_list> lights;
    camera cam;
    color bg;
    int spp;
    int depth;
};

hittable_list random_scene(shared_ptr<hittable_list> &lights) {
    hittable_list world;

    shared_ptr<texture> checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    shared_ptr<material> m_ground = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, m_ground));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center1 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center1, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    shared_ptr<material> m1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, m1));

    shared_ptr<material> m2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-2, 1, 1), 1.0, m2));

    shared_ptr<material> m3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.2);
    world.add(make_shared<sphere>(point3(2, 1, 1), 1.0, m3));

    return world;
}

hittable_list two_spheres(shared_ptr<hittable_list> &lights) {
    hittable_list objects;
    shared_ptr<texture> checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    shared_ptr<material> m = make_shared<lambertian>(checker);
    objects.add(make_shared<sphere>(point3(0, -10, 0), 10, m));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, m));
    return objects;
}

hittable_list two_perlin_spheres(shared_ptr<hittable_list> &lights) {
    hittable_list objects;
    shared_ptr<texture> pertext = make_shared<noise_texture>(4);
    shared_ptr<material> m = make_shared<lambertian>(pertext);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, m));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, m));
    return objects;
}

hittable_list earth(shared_ptr<hittable_list> &lights) {
    hittable_list objects;
    shared_ptr<texture> earth_texture = make_shared<image_texture>("../earthmap.jpg");
    shared_ptr<material> earth_material = make_shared<lambertian>(earth_texture);
    objects.add(make_shared<sphere>(point3(0, 0, 0), 2, earth_material));

    return objects;
}

hittable_list simple_light(shared_ptr<hittable_list> &lights) {
    hittable_list objects;
    shared_ptr<texture> light_color = make_shared<solid_color>(color(4, 4, 4));
    shared_ptr<material> difflight = make_shared<diffuse_light>(light_color);

    auto light1 = make_shared<xy_rect>(3, 5, 1, 3, -2, difflight);
    auto light2 = make_shared<sphere>(point3(0, 7, 0), 2, difflight);
    lights->add(light1);
    lights->add(light2);
    objects.add(light1);
    objects.add(light2);

    shared_ptr<texture> pertext = make_shared<noise_texture>(4);
    shared_ptr<material> m_sphere = make_shared<lambertian>(pertext);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, m_sphere));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, m_sphere));

    return objects;
}

hittable_list cornell_box(shared_ptr<hittable_list> &lights) {
    hittable_list objects;

    shared_ptr<material> red = make_shared<lambertian>(color(.65, .05, .05));
    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.4);
    shared_ptr<material> white = make_shared<lambertian>(color(.73));
    shared_ptr<material> green = make_shared<lambertian>(color(.12, .45, .15));
    shared_ptr<texture> light_color = make_shared<solid_color>(color(15));
    shared_ptr<material> light_mat = make_shared<diffuse_light>(light_color);

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    auto light = make_shared<xz_rect>(213, 343, 227, 332, 554, light_mat);
    lights->add(light);
    objects.add(make_shared<flip_face>(light));

//    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), aluminum);
    box1 = make_shared<axis_rotate>(box1, 1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);


    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(190,90,190), 90 , glass));

//    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
//    box2 = make_shared<axis_rotate>(box2, 1, -18);
//    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
//    objects.add(box2);

    return objects;
}

hittable_list cornell_smoke(shared_ptr<hittable_list> &lights) {
    hittable_list objects;
    shared_ptr<material> red = make_shared<lambertian>(color(.65, .05, .05));
    shared_ptr<material> white = make_shared<lambertian>(color(.73));
    shared_ptr<material> green = make_shared<lambertian>(color(.12, .45, .15));
    shared_ptr<texture> light_color = make_shared<solid_color>(color(7));
    shared_ptr<material> light_mat = make_shared<diffuse_light>(light_color);

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    auto light = make_shared<xz_rect>(213, 343, 227, 332, 554, light_mat);
    lights->add(light);
    objects.add(make_shared<flip_face>(light));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<axis_rotate>(box1, 1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<axis_rotate>(box2, 1, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    return objects;
}

hittable_list final_scene(shared_ptr<hittable_list> &lights) {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light_mat = make_shared<diffuse_light>(make_shared<solid_color>(color(7)));
    auto light = make_shared<xz_rect>(123, 423, 147, 412, 554, light_mat);
    lights->add(light);
    objects.add(make_shared<flip_face>(light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    shared_ptr<material> moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
            point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("../earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(make_shared<translate>(
            make_shared<axis_rotate>(
                    make_shared<bvh_node>(boxes2, 0.0, 1.0), 1, 15),
            vec3(-100, 270, 395))
    );

    return objects;
}

image select_image(std::string scene_name ) {

    // resolution
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    const int max_color_value = 255;
    int samples_per_pixel = 100;
    const int max_depth = 50;

    // default camera setting
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto time0 = 0.0;
    auto time1 = 1.0;

    // world & camera
    color bg(0.7, 0.8, 1.0);
    bvh_node world;
    auto lights = make_shared<hittable_list>();
    if (scene_name.empty() || scene_name == "random") {
        lookfrom = {0, 3, 16};
        image_width = 800;
        image_height = static_cast<int>(image_width / aspect_ratio);
        vfov = 20.0;
        aperture = 0.1;
        world = bvh_node(random_scene(lights), time0, time1);
        samples_per_pixel = 1000;
    } else if (scene_name == "two_spheres") {
        vfov = 20.0;
        world = bvh_node(two_spheres(lights), time0, time1);
    } else if (scene_name == "perlin_spheres") {
        vfov = 20.0;
        world = bvh_node(two_perlin_spheres(lights), time0, time1);
    } else if (scene_name == "earth") {
        vfov = 20.0;
        world = bvh_node(earth(lights), time0, time1);
    } else if (scene_name == "lights") {
        bg = color(0.0);
        vfov = 20.0;
        lookfrom = point3(26, 3, 6);
        lookat = point3(0, 2, 0);
        world = bvh_node(simple_light(lights), time0, time1);
    } else if (scene_name == "cornell_box") {
        world = bvh_node(cornell_box(lights), time0, time1);
        aspect_ratio = 1.0;
        image_width = 800;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 1000;
        bg = color(0.0);
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
    } else if (scene_name == "cornell_smoke") {
        world = bvh_node(cornell_smoke(lights), time0, time1);
        aspect_ratio = 1.0;
        image_width = 300;
        image_height = static_cast<int>(image_width / aspect_ratio);
        bg = color(0.0);
        lookfrom = point3(278, 278, -800);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
    } else if (scene_name == "final_scene") {
        world = bvh_node(final_scene(lights), time0, time1);
        aspect_ratio = 1.0;
        image_width = 800;
        image_height = static_cast<int>(image_width / aspect_ratio);
        samples_per_pixel = 1000;
        bg = color(0, 0, 0);
        lookfrom = point3(478, 278, -600);
        lookat = point3(278, 278, 0);
        vfov = 40.0;
    } else {
        std::cerr << "Scene not found, please input the right scene name!" << std::endl;
        std::cerr << "Rendering the Earth scene instead..." << std::endl;
        vfov = 20.0;
        world = bvh_node(earth(lights), time0, time1);
    }

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    return {image_width, image_height, world, lights, cam, bg, samples_per_pixel, max_depth};
}

#endif //RAYTRACINGINONEWEEKEND_IMAGE_H
