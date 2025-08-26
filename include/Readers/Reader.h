#ifndef DDS2IMAGE_READER_H
#define DDS2IMAGE_READER_H

#include "../Helpers/BinaryStream.h"
#include <cmath>
#include <vector>

#define PACK_RGBA(r, g, b, a)                                                                      \
    ((static_cast<uint32_t>(std::round(r)) << 24) | (static_cast<uint32_t>(std::round(g)) << 16) | \
    (static_cast<uint32_t>(std::round(b)) << 8) | (static_cast<uint32_t>(std::round(a))))

/**
 * Data representing an raw image
 */
struct Image {
    uint32_t width;
    uint32_t height;
    std::vector<uint32_t> data;
};

class Reader {
    protected:
    BinaryStream reader;

    /**
     * @brief Processes the data representing the pixels
     *
     * @param data Data representing the pixels
     */
    virtual void convert(std::span<uint32_t> data) = 0;

    public:
    virtual ~Reader() = default;
    explicit Reader(const BinaryStream& r);

    /**
     * @brief Processes the loaded data
     *
     * @return Raw information for an image
     */
    virtual Image process() = 0;

    /**
     * Creates the appropriate Reader from the given binary
     */
    static std::unique_ptr<Reader> create(std::span<const uint8_t> buffer);

    /**
     * Reads the binary data from the given file
     */
    static std::vector<uint8_t> fromFile(std::string_view file);
};


#endif // DDS2IMAGE_READER_H
