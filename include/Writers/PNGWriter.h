#ifndef DDS2IMAGE_PNGWRITER_H
#define DDS2IMAGE_PNGWRITER_H

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../lib/stb_image_write.h"
#include "Writer.h"

class PNGWriter : public Writer {
    protected:
    std::vector<uint8_t> process() override {
        if (image.width > static_cast<uint32_t>(std::numeric_limits<int>::max()) ||
        image.height > static_cast<uint32_t>(std::numeric_limits<int>::max()))
            throw std::runtime_error("Image dimensions too large");

        const int width  = static_cast<int>(image.width);
        const int height = static_cast<int>(image.height);

        std::vector<uint8_t> pixels;
        pixels.reserve(width * height * 4);

        for (const uint32_t pixel : image.data) {
            uint8_t r = pixel >> 24 & 0xFF;
            uint8_t g = pixel >> 16 & 0xFF;
            uint8_t b = pixel >> 8 & 0xFF;
            uint8_t a = pixel & 0xFF;

            pixels.push_back(r);
            pixels.push_back(g);
            pixels.push_back(b);
            pixels.push_back(a);
        }

        std::vector<uint8_t> buffer;

        auto writeFunc = [](void* context, void* data, const int size) {
            auto* vec   = static_cast<std::vector<uint8_t>*>(context);
            auto* bytes = static_cast<uint8_t*>(data);
            vec->insert(vec->end(), bytes, bytes + size);
        };

        if (!stbi_write_png_to_func(writeFunc, &buffer, width, height, 4, pixels.data(), width * 4))
            throw std::runtime_error("Failed to encode PNG");

        return buffer;
    }

    public:
    explicit PNGWriter(const Image& i) : Writer(i) {
    }
};

#endif // DDS2IMAGE_PNGWRITER_H
