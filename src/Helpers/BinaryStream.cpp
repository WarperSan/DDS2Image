#include "Helpers/BinaryStream.h"
#include <stdexcept>

void BinaryStream::canRead(const size_t bytes) const {
    if (position + bytes <= buffer.size())
        return;

    throw std::out_of_range("Tried to read bytes outside of the buffer size.");
}

void BinaryStream::advance(const size_t bytes) {
    position += bytes;
}

BinaryStream::BinaryStream(const std::span<const uint8_t> buf) : buffer(buf), position(0) {
}

std::string BinaryStream::readFixedString(const size_t length) {
    canRead(length);
    std::string str(reinterpret_cast<const char*>(&buffer[position]), length);
    advance(length);
    return str;
}