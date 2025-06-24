#include <fstream>
#include <vector>
#include <memory>

#include "../include/Converters/Converter.h"
#include "../include/Converters/ConverterFactory.h"

#include "../include/Encoders/Encoder.h"
#include "../include/Encoders/PNGEncoder.h"

std::vector<uint8_t> readFromFile(const std::string &file)
{
    std::ifstream stream(file, std::ios::binary | std::ios::ate);

    if (!stream)
        throw std::runtime_error("Failed to open file: " + file);

    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!stream.read(reinterpret_cast<char *>(buffer.data()), size))
        throw std::runtime_error("Failed to read file: " + file);

    return buffer;
}

void savePNG(const std::string &filename, const std::vector<uint32_t> &rgbaPixels, int width, int height)
{
    std::vector<uint8_t> data;
    data.reserve(width * height * 4);

    for (uint32_t pixel : rgbaPixels)
    {
        uint8_t r = (pixel >> 24) & 0xFF;
        uint8_t g = (pixel >> 16) & 0xFF;
        uint8_t b = (pixel >> 8) & 0xFF;
        uint8_t a = pixel & 0xFF;

        data.push_back(r);
        data.push_back(g);
        data.push_back(b);
        data.push_back(a);
    }

    stbi_write_png(filename.c_str(), width, height, 4, data.data(), width * 4);
}

int main(int argc, char *argv[])
{
    const std::string input = argv[1];
    const std::string output = argv[2];

    // Read file to buffer
    std::vector<uint8_t> buffer = readFromFile(input);

    // Convert buffer
    std::unique_ptr<Converter> converter = ConverterFactory::create(buffer);
    const Image image = converter.get()->process();

    PNGEncoder encoder = PNGEncoder(image.data, image.width, image.height);
    encoder.process("output.png");

    return 0;
    // Output
    savePNG(output, result.data, result.width, result.height);

    return 0;
}