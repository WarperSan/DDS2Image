#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/Readers/Factory.h"
#include "../include/Readers/Reader.h"
#include "../lib/stb_image_write.h"
#include <memory>
#include <span>
#include <string_view>
#include <vector>

void savePNG(const std::string_view filename, std::span<uint32_t> rgbaPixels, const int width, const int height) {
    std::vector<uint8_t> data;
    data.reserve(width * height * 4);

    for (const uint32_t pixel : rgbaPixels) {
        uint8_t r = pixel >> 24 & 0xFF;
        uint8_t g = pixel >> 16 & 0xFF;
        uint8_t b = pixel >> 8 & 0xFF;
        uint8_t a = pixel & 0xFF;

        data.push_back(r);
        data.push_back(g);
        data.push_back(b);
        data.push_back(a);
    }

    stbi_write_png(filename.data(), width, height, 4, data.data(), width * 4);
}

int main(int argc, char* argv[]) {
    const std::string_view input  = argv[1];
    const std::string_view output = argv[2];

    // Read file to buffer
    std::vector<uint8_t> buffer = Reader::fromFile(input);

    // Convert buffer
    const std::unique_ptr<Reader> reader = Factory::create(buffer);

    auto [width, height, data] = reader->process();

    // Output
    savePNG(output, data, static_cast<int>(width), static_cast<int>(height));

    return 0;
}