#include "../../include/Converters/ConverterFactory.h"
#include "../../include/Converters/ATCAConverter.h"
#include "../../include/Converters/ATCIConverter.h"
#include "../../include/Converters/Converter.h"
#include "../../include/Helpers/BinaryReader.h"
#include <stdexcept>

#define MAGIC_NUMBER "DDS "
#define HEADER_SIZE 124
#define PIXEL_FORMAT_SIZE 32

Header ConverterFactory::processHeader(BinaryReader& reader) {
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

PixelFormat ConverterFactory::processPixelFormat(BinaryReader& reader) {
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

std::unique_ptr<Converter> ConverterFactory::create(const std::vector<uint8_t>& buffer) {
    BinaryReader reader(buffer);

    if (reader.readFixedString(4) != MAGIC_NUMBER)
        throw std::runtime_error("Invalid file type.");

    const Header header       = processHeader(reader);
    const std::string& fourCC = header.pixelFormat.fourCC;

    if (fourCC == "ATCI")
        return std::make_unique<ATCIConverter>(reader, header);

    if (fourCC == "ATCA")
        return std::make_unique<ATCAConverter>(reader, header);

    throw std::runtime_error("Unsupported format: " + fourCC);
}
