#include "BinaryReader.h"
#include "Converter.h"
#include <memory>

class Factory
{
private:
    /**
     * @brief Processes the standard DDS header
     *
     * @return Header Metadata found
     */
    static Header processHeader(BinaryReader &reader);

    /**
     * @brief Processes the standard DDS pixel format
     *
     * @return PixelFormat Metadata found
     */
    static PixelFormat processPixelFormat(BinaryReader &reader);

public:
    /**
     * @brief Creates the appropriate converter based of the given data
     *
     * @return std::unique_ptr<Converter> Pointer to the converter
     */
    static std::unique_ptr<Converter> create( std::span<const uint8_t> buffer);
};