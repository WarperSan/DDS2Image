#include "../include/Converter.h"

Result Converter::process()
{
    std::vector<uint32_t> buffer(header.width * header.height);

    processPixels(buffer);

    Result result;

    result.width = header.width;
    result.height = header.height;
    result.data = std::move(buffer);

    return result;
}