#include "../../../include/Readers/DDS/DDSReader.h"
#include <utility>

#define HEADER_SIZE 124
#define PIXEL_FORMAT_SIZE 32

PixelFormat DDSReader::processPixelFormat(BinaryStream& reader) {
    if (reader.read<uint32_t>() != PIXEL_FORMAT_SIZE)
        throw std::runtime_error("Invalid pixel format size.");

    PixelFormat pixelFormat;

    pixelFormat.flags     = reader.read<uint32_t>();
    pixelFormat.fourCC    = reader.readFixedString(4);
    pixelFormat.bpp       = reader.read<uint32_t>();
    pixelFormat.redMask   = reader.read<uint32_t>();
    pixelFormat.greenMask = reader.read<uint32_t>();
    pixelFormat.blueMask  = reader.read<uint32_t>();
    pixelFormat.alphaMask = reader.read<uint32_t>();

    return pixelFormat;
}

Header DDSReader::processHeader(BinaryStream& reader) {
    if (reader.read<uint32_t>() != HEADER_SIZE)
        throw std::runtime_error("Invalid header size.");

    Header header;

    header.flags         = reader.read<int32_t>();
    header.height        = reader.read<uint32_t>();
    header.width         = reader.read<uint32_t>();
    header.pitcOrLinsize = reader.read<uint32_t>();
    header.depth         = reader.read<uint32_t>();
    header.mipMapCount   = reader.read<uint32_t>();

    reader.advance(44);

    header.pixelFormat              = processPixelFormat(reader);
    header.surfaceComplexity        = reader.read<int32_t>();
    header.additionalSurfaceDetails = reader.read<int32_t>();

    reader.advance(12);

    return header;
}

Image DDSReader::process() {
    std::vector<uint32_t> buffer(header.width * header.height);

    convert(buffer);

    Image image;

    image.width  = header.width;
    image.height = header.height;
    image.data   = buffer;

    return image;
}

DDSReader::DDSReader(const BinaryStream& r, Header h) : Reader(r), header(std::move(h)) {
}