#include "../../include/Helpers/BinaryReader.h"
#include <vector>
#include <stdexcept>
#include <cstring>

void BinaryReader::canRead(size_t bytes)
{
    if (position + bytes <= buffer.size())
        return;

    throw std::runtime_error("Out of bounds read at position " + std::to_string(position) + ", requested bytes: " + std::to_string(bytes) + ", buffer size: " + std::to_string(buffer.size()));
}

template <typename T>
inline T BinaryReader::read()
{
    static_assert(std::is_integral_v<T>, "read() supports integral types only");

    canRead(sizeof(T));

    T value = 0;
    std::memcpy(&value, &buffer[position], sizeof(T));

    advance(sizeof(T));

    return value;
}

void BinaryReader::advance(size_t bytes)
{
    position += bytes;
}

BinaryReader::BinaryReader(const std::vector<uint8_t> &buf) : buffer(buf), position(0) {}

std::string BinaryReader::readFixedString(size_t length)
{
    canRead(length);
    std::string str(reinterpret_cast<const char *>(&buffer[position]), length);
    advance(length);
    return str;
}

uint16_t BinaryReader::readUShort()
{
    return read<uint16_t>();
}

uint32_t BinaryReader::readUInt()
{
    return read<uint32_t>();
}

int32_t BinaryReader::readInt()
{
    return read<int32_t>();
}

uint64_t BinaryReader::readULong()
{
    return read<uint64_t>();
}
