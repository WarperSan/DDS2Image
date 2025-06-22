#include "../../include/Helpers/BinaryReader.h"
#include <vector>
#include <stdexcept>

void BinaryReader::canRead(size_t bytes)
{
    if (position + bytes <= buffer.size())
        return;

    throw std::runtime_error("Out of bounds read.");
}

void BinaryReader::advance(size_t bytes)
{
    position += bytes;
}

BinaryReader::BinaryReader(const std::vector<uint8_t> &buf) : buffer(buf), position(0) {}

std::string BinaryReader::readFixedString(size_t length)
{
    canRead(length);
    std::string str(buffer.begin() + position, buffer.begin() + position + length);
    advance(length);
    return str;
}

uint16_t BinaryReader::readUShort()
{
    canRead(2);
    uint16_t value =
        buffer[position] | buffer[position + 1] << 8;
    advance(2);
    return value;
}

uint32_t BinaryReader::readUInt()
{
    canRead(4);
    uint32_t value = buffer[position] | buffer[position + 1] << 8 | buffer[position + 2] << 16 | buffer[position + 3] << 24;
    advance(4);
    return value;
}

int32_t BinaryReader::readInt()
{
    return static_cast<int32_t>(readUInt());
}

uint64_t BinaryReader::readULong()
{
    canRead(8);
    uint64_t value = static_cast<uint64_t>(buffer[position]) | static_cast<uint64_t>(buffer[position + 1]) << 8 | static_cast<uint64_t>(buffer[position + 2]) << 16 | static_cast<uint64_t>(buffer[position + 3]) << 24 | static_cast<uint64_t>(buffer[position + 4]) << 32 | static_cast<uint64_t>(buffer[position + 5]) << 40 | static_cast<uint64_t>(buffer[position + 6]) << 48 | static_cast<uint64_t>(buffer[position + 7]) << 56;
    advance(8);
    return value;
}
