#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: dds2png <input.dds> <output.png>\n";
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];

    // TODO: Your DDS-to-PNG conversion logic

    std::cout << "Converted " << inputFile << " to " << outputFile << "\n";
    return 0;
}