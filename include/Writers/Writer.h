#ifndef DDS2IMAGE_WRITER_H
#define DDS2IMAGE_WRITER_H

#include "Readers/Reader.h"

class Writer {
    protected:
    Image image;

    public:
    virtual ~Writer() = default;
    explicit Writer(Image i);

    /**
     * @brief Processes the loaded data
     *
     * @return Binary data of the image
     */
    virtual std::vector<uint8_t> process() = 0;

    /**
     * Writes the binary data to the given file
     */
    static void toFile(const std::string_view& fileName, const std::vector<uint8_t>& data);

    /**
     * Creates the appropriate Writer for the target type
     */
    static std::unique_ptr<Writer> create(std::string_view fileType, const Image& image);
};

#endif // DDS2IMAGE_WRITER_H
