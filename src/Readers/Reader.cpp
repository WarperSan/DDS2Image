#include "../../include/Readers/Reader.h"
#include <fstream>

Reader::Reader(const BinaryStream& r) : reader(r) {
}

std::vector<uint8_t> Reader::fromFile(const std::string_view file) {
    std::ifstream steam(file.data(), std::ios::binary | std::ios::ate);

    if (!steam)
        throw std::runtime_error("Failed to open file.");

    const std::streamsize size = steam.tellg();
    steam.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!steam.read(reinterpret_cast<char*>(buffer.data()), size))
        throw std::runtime_error("Failed to read file.");

    return buffer;
}