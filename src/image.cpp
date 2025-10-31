#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "image.hpp"
#include <iostream>

Image loadImage(const std::string &filename) {
    Image img;
    int n; // number of color channels

    unsigned char *data = stbi_load(filename.c_str(), &img.width, &img.height, &n, 3);

    // ⬇️ Add this block right here
    if (!data) {
        std::cerr << "Error: could not load image: " << filename << std::endl;
        system("pwd");  // <-- this line prints your current working directory
        return img;
    }

    // Copy pixel data into our struct
    img.data.resize(img.width * img.height);
    for (int i = 0; i < img.width * img.height; ++i) {
        img.data[i].r = data[i * 3 + 0];
        img.data[i].g = data[i * 3 + 1];
        img.data[i].b = data[i * 3 + 2];
    }

    stbi_image_free(data);
    return img;
}
