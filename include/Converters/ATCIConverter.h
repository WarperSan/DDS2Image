#include "Converter.h"
#include <cmath>

#define BLOCK_SIZE 4
#define SCALE_5 255.f / 31.f
#define SCALE_6 255.f / 63.f

class ATCIConverter : public Converter
{
protected:
    void convert(std::vector<uint32_t> *data) override
    {
        uint8_t alphaTable[8];
        uint32_t colorTable[4];

        const uint32_t height = header.height;
        const uint32_t width = header.width;

        for (uint32_t y = 0; y < height; y += BLOCK_SIZE)
        {
            for (uint32_t x = 0; x < width; x += BLOCK_SIZE)
            {
                const uint64_t alphaBits = reader.read<uint64_t>();
                const uint8_t startAlpha = alphaBits & 0xFF;
                const uint8_t endAlpha = (alphaBits >> 8) & 0xFF;
                const uint64_t alphaMask = (alphaBits >> 16) & 0xFFFFFFFFFFFF;

                alphaTable[0] = startAlpha;
                alphaTable[1] = endAlpha;

                if (startAlpha > endAlpha)
                {
                    alphaTable[2] = (6 * startAlpha + 1 * endAlpha) / 7;
                    alphaTable[3] = (5 * startAlpha + 2 * endAlpha) / 7;
                    alphaTable[4] = (4 * startAlpha + 3 * endAlpha) / 7;
                    alphaTable[5] = (3 * startAlpha + 4 * endAlpha) / 7;
                    alphaTable[6] = (2 * startAlpha + 5 * endAlpha) / 7;
                    alphaTable[7] = (1 * startAlpha + 6 * endAlpha) / 7;
                }
                else
                {
                    alphaTable[2] = (4 * startAlpha + 1 * endAlpha) / 5;
                    alphaTable[3] = (3 * startAlpha + 2 * endAlpha) / 5;
                    alphaTable[4] = (2 * startAlpha + 3 * endAlpha) / 5;
                    alphaTable[5] = (1 * startAlpha + 4 * endAlpha) / 5;
                    alphaTable[6] = 0;
                    alphaTable[7] = 255;
                }

                const uint16_t c0 = reader.read<uint16_t>();
                const uint16_t c1 = reader.read<uint16_t>();
                const uint32_t colorIndices = reader.read<uint32_t>();

                const float c0b = (c0 & 31) * SCALE_5;
                const float c0g = ((c0 >> 5) & 31) * SCALE_5;
                const float c0r = ((c0 >> 10) & 31) * SCALE_5;

                const float c1b = (c1 & 31) * SCALE_5;
                const float c1g = ((c1 >> 5) & 63) * SCALE_6;
                const float c1r = ((c1 >> 11) & 31) * SCALE_5;

                colorTable[0] = PACK_RGBA(c0r, c0g, c0b, 0);
                colorTable[1] = PACK_RGBA(
                    (2.f * c0r + c1r) / 3.f,
                    (2.f * c0g + c1g) / 3.f,
                    (2.f * c0b + c1b) / 3.f,
                    0);
                colorTable[2] = PACK_RGBA(
                    (c0r + 2.f * c1r) / 3.f,
                    (c0g + 2.f * c1g) / 3.f,
                    (c0b + 2.f * c1b) / 3.f,
                    0);
                colorTable[3] = PACK_RGBA(c1r, c1g, c1b, 0);

                for (uint32_t bY = 0; bY < BLOCK_SIZE && y + bY < height; ++bY)
                {
                    for (uint32_t bX = 0; bX < BLOCK_SIZE && x + bX < width; ++bX)
                    {
                        const uint32_t index = bY * BLOCK_SIZE + bX;

                        const uint32_t colorIndex = (colorIndices >> (index * 2)) & 0x3;
                        const uint64_t alphaIndex = (alphaMask >> (index * 3)) & 0x7;

                        const uint32_t destIndex = (y + bY) * width + (x + bX);

                        data->at(destIndex) = colorTable[colorIndex] & 0xFFFFFF00 | alphaTable[alphaIndex];
                    }
                }
            }
        }
    }

public:
    ATCIConverter(BinaryReader &r, const Header &h) : Converter(r, h) {}
};