#include "../include/Factory.h"
#include "../include/BinaryReader.h"
#include "../include/Converter.h"
#include "../include/Converters/ATCIConverter.h"
#include <stdexcept>

#define MAGIC_NUMBER "DDS "
#define HEADER_SIZE 124
#define PIXEL_FORMAT_SIZE 32

Header Factory::processHeader(BinaryReader &reader)
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

    header.pixelFormat = processPixelFormat(reader);
    header.surfaceComplexity = reader.readInt();
    header.additionalSurfaceDetails = reader.readInt();

    reader.advance(12);

    return header;
}

PixelFormat Factory::processPixelFormat(BinaryReader &reader)
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

std::unique_ptr<Converter> Factory::create(const std::vector<uint8_t>& buffer)
{
    BinaryReader reader(buffer);

    if (reader.readFixedString(4) != MAGIC_NUMBER)
        throw std::runtime_error("Invalid file type.");

    const Header header = processHeader(reader);
    const std::string fourCC = header.pixelFormat.fourCC;

    if (fourCC == "ATCI")
        return std::make_unique<ATCIConverter>(reader, header);

    throw std::runtime_error("Unsupported format.");
}
