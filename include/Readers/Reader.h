#ifndef DDS2IMAGE_READER_H
#define DDS2IMAGE_READER_H

#include "../Helpers/BinaryReader.h"
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
    BinaryReader reader;

    /**
     * @brief Processes the data representing the pixels
     *
     * @param data Data representing the pixels
     */
    virtual void convert(std::span<uint32_t> data) = 0;

    public:
    virtual ~Reader() = default;

    /**
     * @brief Construct a new Converter object
     *
     * @param r Reader to read the data from
     */
    explicit Reader(const BinaryReader& r);

    /**
     * @brief Processes the loaded data
     *
     * @return Result Raw information for an image
     */
    virtual Image process() = 0;
};


#endif // DDS2IMAGE_READER_H
