#include "../include/BinaryReader.h"
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

uint32_t BinaryReader::readUInt()
{
    canRead(4);
    uint32_t value = 
        buffer[position]
        | buffer[position + 1] << 8
        | buffer[position + 2] << 16
        | buffer[position + 3] << 24;
    advance(4);
    return value;
}

int32_t BinaryReader::readInt()
{
    return static_cast<int32_t>(readUInt());
}
