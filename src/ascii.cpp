#include "ascii.hpp"
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

string imageToASCII(const Image &img, int newWidth, bool color) {
    // Aspect ratio compensation
    float aspect = 0.52f; // tuned for sharper proportions in terminal
    int newHeight = static_cast<int>(img.height * aspect * newWidth / img.width);

    // --- Bilinear interpolation resize ---
    vector<Pixel> resized(newWidth * newHeight);
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            float gx = (x + 0.5f) * img.width / newWidth - 0.5f;
            float gy = (y + 0.5f) * img.height / newHeight - 0.5f;

            int gxi = floor(gx);
            int gyi = floor(gy);
            float cxf = gx - gxi;
            float cyf = gy - gyi;

            auto getPix = [&](int px, int py) -> Pixel {
                px = clamp(px, 0, img.width - 1);
                py = clamp(py, 0, img.height - 1);
                return img.data[py * img.width + px];
            };

            Pixel p1 = getPix(gxi, gyi);
            Pixel p2 = getPix(gxi + 1, gyi);
            Pixel p3 = getPix(gxi, gyi + 1);
            Pixel p4 = getPix(gxi + 1, gyi + 1);

            Pixel result;
            result.r = (1 - cxf) * (1 - cyf) * p1.r + cxf * (1 - cyf) * p2.r + (1 - cxf) * cyf * p3.r + cxf * cyf * p4.r;
            result.g = (1 - cxf) * (1 - cyf) * p1.g + cxf * (1 - cyf) * p2.g + (1 - cxf) * cyf * p3.g + cxf * cyf * p4.g;
            result.b = (1 - cxf) * (1 - cyf) * p1.b + cxf * (1 - cyf) * p2.b + (1 - cxf) * cyf * p3.b + cxf * cyf * p4.b;

            resized[y * newWidth + x] = result;
        }
    }

    // --- Adaptive contrast enhancement ---
    float minL = 1.0f, maxL = 0.0f;
    vector<float> luminances(resized.size());

    for (size_t i = 0; i < resized.size(); ++i) {
        float L = 0.299f * resized[i].r / 255 + 0.587f * resized[i].g / 255 + 0.114f * resized[i].b / 255;
        luminances[i] = L;
        minL = min(minL, L);
        maxL = max(maxL, L);
    }

    // --- Enhanced contrast mapping ---
    float gamma = 0.9f; // slightly lower gamma => more midtone contrast
    const string chars = "@%#*+=-:. "; // simplified set = better definition
    stringstream out;

    // --- Generate ASCII ---
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int idx = y * newWidth + x;
            Pixel p = resized[idx];
            float L = (luminances[idx] - minL) / (maxL - minL + 1e-6f); // normalize contrast
            L = pow(L, gamma);

            int ci = static_cast<int>(L * (chars.size() - 1));
            ci = clamp(ci, 0, (int)chars.size() - 1);
            char c = chars[chars.size() - 1 - ci]; // dark → bright mapping

            if (color)
                out << "\033[38;2;" << (int)p.r << ";" << (int)p.g << ";" << (int)p.b << "m" << c;
            else
                out << c;
        }
        out << "\033[0m\n";
    }

    return out.str();
}
