#include "../../include/Helpers/BinaryReader.h"
#include <stdexcept>

void BinaryReader::canRead(const size_t bytes) const {
    if (position + bytes <= buffer.size())
        return;

    throw std::runtime_error("Out of bounds read at position " + std::to_string(position) + ", requested bytes: " + std::to_string(bytes) +
    ", buffer size: " + std::to_string(buffer.size()));
}

void BinaryReader::advance(const size_t bytes) {
    position += bytes;
}

BinaryReader::BinaryReader(const std::span<const uint8_t> buf) : buffer(buf), position(0) {
}

std::string BinaryReader::readFixedString(const size_t length) {
    canRead(length);
    std::string str(reinterpret_cast<const char*>(&buffer[position]), length);
    advance(length);
    return str;
}