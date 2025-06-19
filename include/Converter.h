#include "BinaryReader.h"
#include <vector>
#include <cstdint>

struct PixelFormat {
    uint32_t flags;
    std::string fourCC;
    uint32_t bpp;
    uint32_t redMask;
    uint32_t greenMask;
    uint32_t blueMask;
    uint32_t alphaMask;
};

struct Header {
    uint32_t flags;
    uint32_t height;
    uint32_t width;
    uint32_t pitcOrLinsize;
    uint32_t depth;
    uint32_t mipMapCount;
    PixelFormat pixelFormat;
    int32_t surfaceComplexity;
    int32_t additionalSurfaceDetails;
};

struct Result {
    uint32_t width;
    uint32_t height;
    std::vector<uint32_t> data;
};

class Converter {
    private:
        BinaryReader reader;

        /**
         * @brief Prcesses the standard DDS header
         * 
         * @return Header Metadata found
         */
        Header processHeader();

        /**
         * @brief Processes the standard DDS pixel format
         * 
         * @return PixelFormat Metadata found
         */
        PixelFormat processPixelFormat();

    protected:
        /**
         * @brief Processes the data representing the pixels
         * 
         * @param data Data representing the pixels
         */
        virtual void processPixels(std::vector<uint32_t>* data) = 0;

    public:
        /**
         * @brief Construct a new Converter object
         * 
         * @param r Reader to read the data from
         */
        Converter(BinaryReader& r) : reader(r) {}

        /**
         * @brief Porcesses the loaded data
         * 
         * @return Result Raw information for an image
         */
        Result process();
};