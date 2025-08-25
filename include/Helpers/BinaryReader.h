#pragma once

#include <bit>
#include <cstring>
#include <fstream>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

/**
 * @brief Wrapper to handle binary arrays
 */
class BinaryReader {
    private:
    std::span<const uint8_t> buffer;
    size_t position;

    /**
     * @brief Checks if the reader can read the amount of bytes requested
     *
     * @param bytes Amount of bytes to read
     */
    void canRead(size_t bytes);

    public:
    /**
     * @brief Construct a new Binary Reader object
     *
     * @param buf Raw data to read from
     */
    BinaryReader(std::span<const uint8_t> buf);

    /**
     * @brief Advances the current position by the given amount of bytes
     *
     * @param bytes Amount of bytes to advance by
     */
    void advance(size_t bytes);

    /**
     * @brief Reads the given value
     *
     * @tparam T Type of the value to read
     * @return T Value read
     */
    template <typename T> T read() {
        static_assert(std::is_integral_v<T>, "read() supports integral types only");

        canRead(sizeof(T));

        T value = 0;
        memcpy(&value, &buffer[position], sizeof(T));

        advance(sizeof(T));

        return value;
    }

    /**
     * @brief Reads a string with the given length
     *
     * @param length Length of the string to read
     * @return std::string String read
     */
    std::string readFixedString(size_t length);
};