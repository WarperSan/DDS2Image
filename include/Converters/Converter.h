#pragma once

#include "../Helpers/BinaryReader.h"
#include <vector>
#include <cstdint>

#define PACK_RGBA(r, g, b, a)                       \
    ((static_cast<uint32_t>(std::round(r)) << 24) | \
     (static_cast<uint32_t>(std::round(g)) << 16) | \
     (static_cast<uint32_t>(std::round(b)) << 8) |  \
     (static_cast<uint32_t>(std::round(a))))

struct PixelFormat
{
    uint32_t flags;
    std::string fourCC;
    uint32_t bpp;
    uint32_t redMask;
    uint32_t greenMask;
    uint32_t blueMask;
    uint32_t alphaMask;
};

struct Header
{
    uint32_t flags;
    uint32_t height;
    uint32_t width;
    uint32_t pitcOrLinsize;
    uint32_t depth;
    uint32_t mipMapCount;
    PixelFormat pixelFormat;
    int32_t surfaceComplexity;
    int32_t additionalSurfaceDetails;
};

struct Image
{
    uint32_t width;
    uint32_t height;
    std::vector<uint32_t> data;
};

class Converter
{
protected:
    BinaryReader reader;
    const Header header;

    /**
     * @brief Converts the raw data representing the pixels into a standard form
     *
     * @param data Data representing the pixels
     */
    virtual void convert(std::vector<uint32_t> *data) = 0;

public:
    /**
     * @brief Construct a new Converter object
     *
     * @param r Reader to read the data from
     * @param h Metadata loaded from the data
     */
    Converter(BinaryReader &r, const Header &h) : reader(r), header(h) {}

    /**
     * @brief Processes the loaded data
     *
     * @return Image Raw information for an image
     */
    const Image process();
};