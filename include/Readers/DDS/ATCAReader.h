#ifndef DDS2IMAGE_ATCAREADER_H
#define DDS2IMAGE_ATCAREADER_H

#include "DDSReader.h"
#include <cmath>

#define BLOCK_SIZE 4
#define SCALE_5 (255.f / 31.f)
#define SCALE_6 (255.f / 63.f)

class ATCAReader : public DDSReader {
    protected:
    void convert(std::span<uint32_t> data) override {
        const uint32_t height = header.height;
        const uint32_t width  = header.width;

        const size_t maxY         = (static_cast<size_t>(height) - 1) / BLOCK_SIZE * BLOCK_SIZE;
        const size_t maxX         = (static_cast<size_t>(width) - 1) / BLOCK_SIZE * BLOCK_SIZE;
        const size_t maxDestIndex = (maxY + (BLOCK_SIZE - 1)) * width + (maxX + (BLOCK_SIZE - 1));

        if (data.size() <= maxDestIndex)
            throw std::out_of_range{ "processPixels buffer was too small" };

        uint32_t colorTable[4]{};

        for (uint32_t y = 0; y < height; y += BLOCK_SIZE) {
            for (uint32_t x = 0; x < width; x += BLOCK_SIZE) {
                const auto alphaBits = reader.read<uint64_t>();

                const auto c0           = reader.read<uint16_t>();
                const auto c1           = reader.read<uint16_t>();
                const auto colorIndices = reader.read<uint32_t>();

                const float c0b = (c0 & 31) * SCALE_5;
                const float c0g = (c0 >> 5 & 31) * SCALE_5;
                const float c0r = (c0 >> 10 & 31) * SCALE_5;

                const float c1b = (c1 & 31) * SCALE_5;
                const float c1g = (c1 >> 5 & 63) * SCALE_6;
                const float c1r = (c1 >> 11 & 31) * SCALE_5;

                colorTable[0] = PACK_RGBA(c0r, c0g, c0b, 0);
                colorTable[1] = PACK_RGBA((2.f * c0r + c1r) / 3.f, (2.f * c0g + c1g) / 3.f, (2.f * c0b + c1b) / 3.f, 0);
                colorTable[2] = PACK_RGBA((c0r + 2.f * c1r) / 3.f, (c0g + 2.f * c1g) / 3.f, (c0b + 2.f * c1b) / 3.f, 0);
                colorTable[3] = PACK_RGBA(c1r, c1g, c1b, 0);

                for (uint32_t bY = 0; bY < BLOCK_SIZE && y + bY < height; ++bY) {
                    for (uint32_t bX = 0; bX < BLOCK_SIZE && x + bX < width; ++bX) {
                        const uint32_t index = bY * BLOCK_SIZE + bX;

                        const uint32_t colorIndex = colorIndices >> (index * 2) & 0x3;

                        const uint8_t alpha4 = alphaBits >> (4 * index) & 0b1111;
                        const uint8_t alpha8 = alpha4 << 4 | alpha4;

                        const uint32_t destIndex = (y + bY) * width + (x + bX);
                        data[destIndex]          = colorTable[colorIndex] & 0xFFFFFF00 | alpha8;
                    }
                }
            }
        }
    }

    public:
    ATCAReader(const BinaryReader& r, const Header& h) : DDSReader(r, h) {
    }
};

#endif // DDS2IMAGE_ATCAREADER_H
