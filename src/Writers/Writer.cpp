#include "../../include/Writers/Writer.h"

#include "Writers/PNGWriter.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Writer::Writer(Image i) : image(std::move(i)) {
}

void Writer::toFile(const std::string_view& fileName, const std::vector<uint8_t>& data) {
    if (data.size() > static_cast<size_t>(std::numeric_limits<std::streamsize>::max()))
        throw std::runtime_error("Buffer too large to write to stream");

    std::ofstream out(std::string(fileName), std::ios::binary);

    if (!out)
        throw std::runtime_error("Failed to open file for writing.");

    const auto size = static_cast<std::streamsize>(data.size());
    out.write(reinterpret_cast<const char*>(data.data()), size);

    if (!out)
        throw std::runtime_error("Failed to write data to file.");
}

std::unique_ptr<Writer> Writer::create(const std::string_view fileType, const Image& image) {
    if (fileType == ".png")
        return std::make_unique<PNGWriter>(image);

    throw std::runtime_error("Unsupported format: " + std::string(fileType));
}