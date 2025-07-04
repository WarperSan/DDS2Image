#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include <fstream>
#include <vector>
#include <memory>

#include "../include/BinaryReader.h"
#include "../include/Converter.h"
#include "../include/Factory.h"

std::vector<uint8_t> readFromFile(std::string file)
{
    std::ifstream steam(file, std::ios::binary | std::ios::ate);

    if (!steam)
        throw std::runtime_error("Failed to open file.");

    std::streamsize size = steam.tellg();
    steam.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!steam.read(reinterpret_cast<char *>(buffer.data()), size))
        throw std::runtime_error("Failed to read file.");

    return buffer;
}

Result convertBuffer(std::vector<uint8_t> buffer)
{
    std::unique_ptr<Converter> converter = Factory::create(buffer);

    return converter.get()->process();
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
    const Result result = convertBuffer(buffer);

    // Output
    savePNG(output, result.data, result.width, result.height);

    return 0;
}