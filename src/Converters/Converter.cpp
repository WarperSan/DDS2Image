#include "../../include/Converters/Converter.h"

const Image Converter::process() {
    std::vector<uint32_t> buffer(header.width * header.height);

    convert(&buffer);

    Image image;

    image.width  = header.width;
    image.height = header.height;
    image.data   = buffer;

    return image;
}