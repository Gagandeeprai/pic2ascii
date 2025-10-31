#include <iostream>
#include <string>
#include <fstream>
#include "image.hpp"
#include "ascii.hpp"
#include "utils.hpp"

using namespace std;

void showHelp() {
    cout << "pic2ascii - Convert images to colorful ASCII art\n\n"
         << "Usage:\n"
         << "  pic2ascii <image_path> [options]\n\n"
         << "Options:\n"
         << "  --width <n>       Set output width (auto-detect by default)\n"
         << "  --grayscale       Output in black & white instead of color\n"
         << "  --output <file>   Save ASCII art to text file\n"
         << "  --help, -h        Show this help message\n\n"
         << "Examples:\n"
         << "  pic2ascii assets/test.jpeg\n"
         << "  pic2ascii mypic.png --width 120 --grayscale\n"
         << "  pic2ascii image.jpg --output out.txt\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2 || string(argv[1]) == "--help" || string(argv[1]) == "-h") {
        showHelp();
        return 0;
    }

    string inputPath = argv[1];
    int width = 0;
    bool color = true;
    string outputFile = "";

    // Parse other args
    for (int i = 2; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--width" && i + 1 < argc) {
            width = stoi(argv[++i]);
        } else if (arg == "--grayscale") {
            color = false;
        } else if (arg == "--output" && i + 1 < argc) {
            outputFile = argv[++i];
        }
    }

    if (width == 0)
        width = getTerminalWidth() / 2;

    Image img = loadImage(inputPath);
    if (img.data.empty()) {
        cerr << "Error: Failed to load image: " << inputPath << endl;
        return 1;
    }

    string asciiArt = imageToASCII(img, width, color);

    if (!outputFile.empty()) {
        ofstream file(outputFile);
        file << asciiArt;
        file.close();
        cout << "Saved output to " << outputFile << endl;
    } else {
        cout << asciiArt << "\033[0m" << endl;
    }

    return 0;
}
