#pragma once

#include <string>
#include <vector>

/**
 * @brief Wrapper to handle binary arrays
 */
class BinaryReader
{
private:
    const std::vector<uint8_t> &buffer;
    size_t position;

    /**
     * @brief Checks if the reader can read the amount of bytes requested
     *
     * @param bytes Amount of bytes to read
     */
    void canRead(size_t bytes);

    /**
     * @brief Reads the given value
     *
     * @tparam T Type of the value to read
     * @return T Value read
     */
    template <typename T>
    T read();

public:
    /**
     * @brief Construct a new Binary Reader object
     *
     * @param buf Raw data to read from
     */
    BinaryReader(const std::vector<uint8_t> &buf);

    /**
     * @brief Advances the current position by the given amount of bytes
     *
     * @param bytes Amount of bytes to advance by
     */
    void advance(size_t bytes);

    /**
     * @brief Reads a string with the given length
     *
     * @param length Length of the string to read
     * @return std::string String read
     */
    std::string readFixedString(size_t length);

    /**
     * @brief Reads an unsigned 16 bits number
     *
     * @return uint16_t Number read
     */
    uint16_t readUShort();

    /**
     * @brief Reads an unsigned 32 bits number
     *
     * @return uint32_t Number read
     */
    uint32_t readUInt();

    /**
     * @brief Reads a signed 32 bits number
     *
     * @return int32_t Number read
     */
    int32_t readInt();

    /**
     * @brief Reads an unsigned 64 bits number
     *
     * @return uint64_t Number read
     */
    uint64_t readULong();
};