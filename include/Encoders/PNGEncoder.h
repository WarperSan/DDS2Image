#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Helpers/stb_image_write.h"

#include "Encoder.h"

class PNGEncoder : public Encoder {
    protected:
    void encode(const std::string& filename) override {
        std::vector<uint8_t> data;
        data.reserve(width * height * 4);

        for (uint32_t pixel : pixels) {
            uint8_t r = (pixel >> 24) & 0xFF;
            uint8_t g = (pixel >> 16) & 0xFF;
            uint8_t b = (pixel >> 8) & 0xFF;
            uint8_t a = pixel & 0xFF;

            data.push_back(r);
            data.push_back(g);
            data.push_back(b);
            data.push_back(a);
        }

        stbi_write_png(filename.c_str(), width, height, 4, data.data(), width * 4);
    }

    public:
    PNGEncoder(const Image& image) : Encoder(image) {
    }
};