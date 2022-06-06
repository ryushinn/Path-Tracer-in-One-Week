//
// Created by Ryushinn on 2020/12/28.
//


#include "rtweekend.h"

#include "hittable_list.h"
#include "camera.h"
#include "lambertian.h"
#include "bvh_node.h"
#include "image.h"
#include <iostream>

color ray_color(const ray &r, const color &background, const hittable &world,
                const shared_ptr<hittable_list> &lights, int depth) {
    if (depth <= 0) {
        return {0.0};
    }

    hit_record rec;
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    scatter_record srec;
    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    double pdf;
    vec3 sampled_directions;
    if (!srec.is_delta && lights->size() != 0) {
        auto light_pdf = make_shared<hittable_pdf>(lights, rec.p);
        mixture_pdf mixed_pdf(light_pdf, srec.pdf_ptr);
        sampled_directions = mixed_pdf.generate();
        pdf = mixed_pdf.value(sampled_directions);
    } else {
        sampled_directions = srec.pdf_ptr->generate();
        pdf = srec.pdf_ptr->value(sampled_directions);
    }

    auto scattered = ray(rec.p, sampled_directions, r.time());

    return emitted + rec.mat_ptr->BxDF(r, scattered, rec) * ray_color(scattered, background, world, lights, depth - 1) / pdf;
}


int main(int argc, char *argv[]) {
    // cmd inputs
    if (argc > 2) {
        std::cerr << "Please input your scene name" << std::endl;
        return -1;
    }
    std::string scene_name;
    if (argc == 2)
        scene_name = argv[1];

    image image = select_image(scene_name);

    bool sanity_check = false;
    if (sanity_check)
        image.spp = 2;


    std::vector<double> buffer(image.width * image.height * 3);

#pragma omp parallel for num_threads(12)
    for (int j = image.height - 1; j >= 0; --j) {
        std::cerr << "\rRendering the scanline: " << j << " " << std::flush;
        for (int i = 0; i <= image.width - 1; ++i) {
            // output color in the reading order.
            color pixel_color(0.0);
            for (int s = 0; s < image.spp; s++) {
                auto u = (double(i) + random_double()) / (image.width - 1);
                auto v = (double(j) + random_double()) / (image.height - 1);
                ray r = image.cam.get_ray(u, v);

                pixel_color += ray_color(r, image.bg, image.world, image.lights, image.depth);
            }
            pixel_color /= image.spp;

            buffer[3 * (j * image.width + i) + 0] = pixel_color[0];
            buffer[3 * (j * image.width + i) + 1] = pixel_color[1];
            buffer[3 * (j * image.width + i) + 2] = pixel_color[2];
        }
    }
    write_image(std::cout, &buffer[0], image.width, image.height);
    std::cerr << "\nDone.\n";
}
