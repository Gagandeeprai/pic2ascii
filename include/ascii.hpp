#pragma once
#include "image.hpp"
#include <string>

std::string imageToASCII(const Image &img, int outputWidth, bool useColor = false);
