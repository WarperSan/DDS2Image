#pragma once

#include "../Converters/Converter.h"

class Encoder {
    protected:
    const std::vector<uint32_t> pixels;
    const uint32_t width;
    const uint32_t height;

    /**
     * @brief Encodes the loaded pixels
     *
     * @param filename Name of the output
     */
    virtual void encode(const std::string& filename) = 0;

    public:
    /**
     * @brief Construct a new Encoder object
     *
     * @param Image Image to encode
     */
    Encoder(const Image& image) : pixels(image.data), width(image.width), height(image.height) {
    }

    /**
     * @brief Processes the loaded pixels
     *
     * @return std::string Path to the output
     */
    void process(const std::string& filename);
};