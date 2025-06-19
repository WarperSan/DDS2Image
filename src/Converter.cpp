#include "../include/Converter.h"
#include <stdexcept>

#define MAGIC_NUMBER "DDS "
#define HEADER_SIZE 124
#define PIXEL_FORMAT_SIZE 32

Header Converter::processHeader()
{
    if (reader.readUInt() != HEADER_SIZE)
        throw std::runtime_error("Invalid header size.");

    Header header;

    header.flags = reader.readInt();
    header.height = reader.readUInt();
    header.width = reader.readUInt();
    header.pitcOrLinsize = reader.readUInt();
    header.depth = reader.readUInt();
    header.mipMapCount = reader.readUInt();

    reader.advance(44);

    header.pixelFormat = processPixelFormat();
    header.surfaceComplexity = reader.readInt();
    header.additionalSurfaceDetails = reader.readInt();

    reader.advance(12);

    return header;
}

PixelFormat Converter::processPixelFormat()
{
    if (reader.readUInt() != PIXEL_FORMAT_SIZE)
        throw std::runtime_error("Invalid pixel format size.");

    PixelFormat pixelFormat;

    pixelFormat.flags = reader.readUInt();
    pixelFormat.fourCC = reader.readFixedString(4);
    pixelFormat.bpp = reader.readUInt();
    pixelFormat.redMask = reader.readUInt();
    pixelFormat.greenMask = reader.readUInt();
    pixelFormat.blueMask = reader.readUInt();
    pixelFormat.alphaMask = reader.readUInt();

    return pixelFormat;
}

Result Converter::process()
{
    if (reader.readFixedString(4) != MAGIC_NUMBER)
        throw std::runtime_error("Invalid file type.");

    const Header header = processHeader();
    std::vector<uint32_t> buffer(header.width * header.height);

    processPixels(&buffer);

    Result result;

    result.width = header.width;
    result.height = header.height;
    result.data = buffer;

    return result;
}
