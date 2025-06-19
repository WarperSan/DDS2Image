#include "BinaryReader.h"
#include <vector>
#include <cstdint>

struct PixelFormat {

};

struct Header {

};

struct Result {

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
        virtual void processPixels(std::vector<uint32_t>* data);

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