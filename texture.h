#ifndef RAYTRACINGINONEWEEKEND_TEXTURE_H
#define RAYTRACINGINONEWEEKEND_TEXTURE_H

#include <utility>

#include "rtweekend.h"
#include "perlin.h"
#include "rtw_stb_image.h"

class texture {
public:
    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture {
private:
    color color_value;
public:
    solid_color() = default;

    explicit solid_color(color c) : color_value(c) {}

    solid_color(double r, double g, double b) : solid_color(color(r, g, b)) {}

    color value(double u, double v, const point3 &p) const override;
};

color solid_color::value(double u, double v, const point3 &p) const {
    return color_value;
}

class checker_texture : public texture {
public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;

    checker_texture() = default;

    checker_texture(shared_ptr<texture> _odd, shared_ptr<texture> _even) : odd(std::move(_odd)),
                                                                           even(std::move(_even)) {}

    checker_texture(color c1, color c2) : odd(make_shared<solid_color>(c1)), even(make_shared<solid_color>(c2)) {}

    color value(double u, double v, const point3 &p) const override;
};

color checker_texture::value(double u, double v, const point3 &p) const {
    auto sines = sin(p.x() * 10) * sin(p.y() * 10) * sin(p.z() * 10);
    if (sines < 0) {
        return odd->value(u, v, p);
    } else {
        return even->value(u, v, p);
    }
}

class noise_texture : public texture {
public:
    perlin noise;
    double scale = 1.0;

    noise_texture() = default;
    explicit noise_texture(double sc) : scale(sc) {}

    color value(double u, double v, const point3& p) const override {
        return 0.5 * color(1.0 + sin(scale * p.z() + 10 * noise.turb(p)));
    }
};

class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;

    unsigned char* data;
    int width, height;
    int bytes_per_scanline;


    image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        bytes_per_scanline = width * bytes_per_pixel;
    }

    ~image_texture() {
        delete data;
    }

    color value(double u, double v, const point3 &p) const override;
};

color image_texture::value(double u, double v, const point3 &p) const {
    if (!data) {
        return color(0, 1, 1);
    }

    u = clamp(u, 0, 1);
    v = 1.0 - clamp(v, 0, 1);

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);
    if (i >= width) i--;
    if (j >= height) j--;

    const auto color_scale = 1.0 / 255.0;
    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return color_scale * color(pixel[0], pixel[1],pixel[2]);
}

#endif //RAYTRACINGINONEWEEKEND_TEXTURE_H
