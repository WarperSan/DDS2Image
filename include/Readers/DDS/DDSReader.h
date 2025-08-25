#ifndef DDS2IMAGE_DDSREADER_H
#define DDS2IMAGE_DDSREADER_H

#include "../Reader.h"

struct PixelFormat {
    uint32_t flags;
    std::string fourCC;
    uint32_t bpp;
    uint32_t redMask;
    uint32_t greenMask;
    uint32_t blueMask;
    uint32_t alphaMask;
};

struct Header {
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

class DDSReader : public Reader {
    protected:
    Header header;

    public:
    explicit DDSReader(const BinaryStream& r, Header h);

    /**
     * Reads the pixel format from the given reader
     */
    static PixelFormat processPixelFormat(BinaryStream& reader);

    /**
     * Reads the header from the given reader
     */
    static Header processHeader(BinaryStream& reader);

    Image process() override;
};

#endif // DDS2IMAGE_DDSREADER_H
