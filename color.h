#ifndef RAYTRACINGINONEWEEKEND_COLOR_H
#define RAYTRACINGINONEWEEKEND_COLOR_H

#include "vec3.h"
#include <iostream>

void write_color(std::ostream &out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // gamma correction 2
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << " "
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << " "
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << "\n";
}

void write_image(std::ostream &out, double *img, int image_width, int image_height){
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; j--) {
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(img[3*(j*image_width+i)+0], img[3*(j*image_width+i)+1], img[3*(j*image_width+i)+2]);
            write_color(out, pixel_color);
        }
    }
}

#endif //RAYTRACINGINONEWEEKEND_COLOR_H
