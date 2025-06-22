#include "../../include/Converters/Converter.h"

const Result Converter::process()
{
    std::vector<uint32_t> buffer(header.width * header.height);

    convert(&buffer);

    Result result;

    result.width = header.width;
    result.height = header.height;
    result.data = buffer;

    return result;
}