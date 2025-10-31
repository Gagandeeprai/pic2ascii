#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

struct Pixel {
    unsigned char r, g, b;
};

struct Image {
    int width;
    int height;
    std::vector<Pixel> data;
};

Image loadImage(const std::string &filename);

#endif
